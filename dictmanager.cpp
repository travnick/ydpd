/***************************************************************************
 *   Copyright (C) 2011-2012 by Marcin Nawrocki <mar.nawrocki@gmail.com>   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "dictmanager.h"
#include "configmanager.h"
#include "dictinstance.h"
#include "ydptypes.h"
#include "dict.h"
#include <QStringListModel>
#include <QStandardItemModel>

typedef QMap<int, IDictionary*> VersionToDictMap;

class DictManager : public IDictManager
{
public:
    static DictManager& instance();
    QAbstractItemModel* entriesModel() const { return m_entriesModel; }
    QAbstractItemModel* dictionaryModel() const { return m_dictionaryModel; }
    QAbstractItemModel* historyModel() const { return m_historyModel; }
    bool isTypeEqual(int type) { return ((m_version + m_direction) == type ? true : false); }
    int currentDictionaryIndex() const;
    bool isCurrentDictValid() const;
    void setVersion(int currentDictionaryIndex);
    void invertDirection();
    bool setType(int historyItemIndex);
    bool isDirectionForeign() const;
    void initHistory();
    void insertHistoryItem(QString text);
    void initDictionary();
    void clearEntries();
    void clearDictionary();
    void clearHistory();
    void initEntries();
    void init();
    const QStringList& entries() const;
    QString description(int index) const;
    int entryToIndex(QString entry) const;
    QString entriesText(int index) const;
    QString foreignIconName() const;
    QString tooltip() const;
    QString wavFormat() const;
    QString samplesPath() const;
    DictManager();
    ~DictManager();
private:
    VersionToDictMap m_dictionaries;
    int m_version;
    int m_direction;
    QStringList m_entriesList;
    QStringListModel* m_entriesModel;
    QStandardItemModel* m_dictionaryModel;
    QStandardItemModel* m_historyModel;
    int findValidDictionaryVersion();
};

DictManager::DictManager() :
    m_version(YdpTypes::InvalidVersion),
    m_direction(YdpTypes::Foreign)
{
    IConfigManager& confMan = IConfigManager::instance();

    foreach (int version, DictInstance::versionList())
    {
        IDictionary* dict = IDictionary::createInstance(version, confMan.config().dictionaryPath(version));
        m_dictionaries[version] = dict;
        QObject::connect(&confMan, SIGNAL(dictionaryDirectoryChanged(int, QString)), dict, SLOT(initialize(int, QString)));
    }

    init();

    m_dictionaryModel = new QStandardItemModel();
    m_historyModel    = new QStandardItemModel();
    m_entriesModel    = new QStringListModel();
    m_entriesModel->setStringList(QStringList());
}

DictManager::~DictManager()
{
    foreach (IDictionary* dictionary, m_dictionaries)
        dictionary->release();
    clearDictionary();
    clearHistory();
    delete m_dictionaryModel;
    delete m_historyModel;
    delete m_entriesModel;
}

IDictManager& IDictManager::instance()
{
    static DictManager manager;
    return manager;
}

int DictManager::findValidDictionaryVersion()
{
    foreach (int version, m_dictionaries.keys())
        if (m_dictionaries[version]->isValid())
            return version;

    return YdpTypes::InvalidVersion;
}

void DictManager::init()
{
    IConfigManager& confMan = IConfigManager::instance();

    if (confMan.config().dictionaryToOpen() == Recent)
    {
        m_version   = YdpTypes::typeToVersion(confMan.config().recentDictionary());
        m_direction = YdpTypes::typeToDirection(confMan.config().recentDictionary());
    }
    else
    {
        m_version   = YdpTypes::typeToVersion(confMan.config().selectedDictionary());
        m_direction = YdpTypes::typeToDirection(confMan.config().selectedDictionary());
    }

    if (YdpTypes::InvalidVersion == m_version || !m_dictionaries[m_version]->isValid())
        m_version = findValidDictionaryVersion();
}

bool DictManager::isCurrentDictValid() const
{
    return m_version != YdpTypes::InvalidVersion;
}

void DictManager::initHistory()
{
    IConfigManager& confMan = IConfigManager::instance();

    foreach (HistoryItem historyItem, confMan.config().history())
    {
        YdpTypes::YdpDirection direction = YdpTypes::typeToDirection(historyItem.type());
        YdpTypes::YdpVersion   version   = YdpTypes::typeToVersion(historyItem.type());
        IDictionary*           dict      = m_dictionaries[version];

        if (!dict->isValid())
            continue;

        if (m_historyModel->rowCount() == confMan.config().maxNumberOfHistoryItems())
            break;

        QStandardItem *item = new QStandardItem();
        item->setText(historyItem.text());
        item->setIcon(QIcon(DictInstance::dictInst(version).iconName(direction)));
        item->setData(QVariant(historyItem.type()), Qt::UserRole);
        m_historyModel->appendRow(item);
    }
}

void DictManager::insertHistoryItem(QString text)
{
    QStandardItem *item = new QStandardItem();
    item->setText(text);
    item->setIcon(QIcon(DictInstance::dictInst(m_version).iconName(m_direction)));
    item->setData(QVariant(m_version + m_direction), Qt::UserRole);
    m_historyModel->insertRow(0, item);
    const int rowCount = m_historyModel->rowCount();
    if (rowCount > IConfigManager::instance().config().maxNumberOfHistoryItems())
    {
        delete m_historyModel->item(rowCount-1);
        m_historyModel->removeRow(rowCount-1);
    }
    IConfigManager::instance().config().insertHistoryItem(text, m_version + m_direction);
}

void DictManager::initDictionary()
{
    foreach (int version, m_dictionaries.keys())
    {
        if (m_dictionaries[version]->isValid())
        {
            QStandardItem *item = new QStandardItem();
            DictInst instance = DictInstance::dictInst(version);
            item->setText(instance.description());
            item->setIcon(QIcon(instance.iconName(YdpTypes::Foreign)));
            item->setData(QVariant(version), Qt::UserRole);
            m_dictionaryModel->appendRow(item);
        }
    }
}

void DictManager::initEntries()
{
    m_entriesModel->setStringList(entries());
    emit dictionaryChanged(m_version + m_direction);
}

void DictManager::clearEntries()
{
    m_entriesModel->setStringList(QStringList());
}

void DictManager::clearDictionary()
{
    const int itemCount = m_dictionaryModel->rowCount();
    for (int index = 0; index < itemCount; index++)
        delete m_dictionaryModel->item(index);
    m_dictionaryModel->removeRows(0, itemCount);
}

void DictManager::clearHistory()
{
    const int itemCount = m_historyModel->rowCount();
    for (int index = 0; index < itemCount; index++)
        delete m_historyModel->item(index);
    m_historyModel->removeRows(0, itemCount);
}

const QStringList& DictManager::entries() const
{
    return m_dictionaries[m_version]->entries(m_direction);
}

QString DictManager::description(int index) const
{
    return m_dictionaries[m_version]->description(m_direction, index);
}

int DictManager::entryToIndex(QString entry) const
{
    return m_dictionaries[m_version]->entryToIndex(m_direction, entry);
}

QString DictManager::entriesText(int index) const
{
    return m_entriesModel->data(m_entriesModel->index(index), Qt::EditRole).toString();
}

QString DictManager::foreignIconName() const
{
    return DictInstance::dictInst(m_version).foreignIconName();
}

QString DictManager::tooltip() const
{
    return DictInstance::dictInst(m_version).toolTip();
}

QString DictManager::wavFormat() const
{
    return DictInstance::dictInst(m_version).wavFormat();
}

void DictManager::invertDirection()
{
    m_direction = YdpTypes::getOppositeDirection(m_direction);
    initEntries();
}

bool DictManager::isDirectionForeign() const
{
    return YdpTypes::Foreign == m_direction;
}

QString DictManager::samplesPath() const
{
    return IConfigManager::instance().config().samplesPath(m_version);
}

void DictManager::setVersion(int dictionaryIndex)
{
    m_version = m_dictionaryModel->itemData(m_dictionaryModel->index(dictionaryIndex, 0))[Qt::UserRole].toInt();
    initEntries();
}

bool DictManager::setType(int historyItemIndex)
{
    const int type = m_historyModel->itemData(m_historyModel->index(historyItemIndex, 0))[Qt::UserRole].toInt();
    const int version = YdpTypes::typeToVersion(type);
    const int direction = YdpTypes::typeToDirection(type);
    const bool versionChange = (m_version != version);
    const bool directionChange = (m_direction != direction);
    m_version = version;
    m_direction = direction;
    if (versionChange || directionChange)
        initEntries();
    return versionChange;
}

int DictManager::currentDictionaryIndex() const
{
    const int count = m_dictionaryModel->rowCount();
    int index;
    for (index = 0; index < count; index++)
        if (m_version == m_dictionaryModel->itemData(m_dictionaryModel->index(index, 0))[Qt::UserRole].toInt())
            break;

    return index;
}
