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
    QAbstractItemModel* entriesModel() const { return _entriesModel; }
    QAbstractItemModel* dictionaryModel() const { return _dictionaryModel; }
    QAbstractItemModel* historyModel() const { return _historyModel; }
    bool isTypeEqual(int type) { return ((_version + _direction) == type ? true : false); }
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
    VersionToDictMap _dictionaries;
    int _version;
    int _direction;
    QStringList _entriesList;
    QStringListModel* _entriesModel;
    QStandardItemModel* _dictionaryModel;
    QStandardItemModel* _historyModel;
    int findValidDictionaryVersion();
};

DictManager::DictManager() :
    _version(YdpTypes::InvalidVersion),
    _direction(YdpTypes::Foreign)
{
    IConfigManager& confMan = IConfigManager::instance();

    foreach (int version, DictInstance::versionList())
    {
        IDictionary* dict = IDictionary::createInstance(version, confMan.config().dictionaryPath(version));
        _dictionaries[version] = dict;
        QObject::connect(&confMan, SIGNAL(dictionaryDirectoryChanged(int, QString)), dict, SLOT(initialize(int, QString)));
    }

    init();

    _dictionaryModel = new QStandardItemModel();
    _historyModel    = new QStandardItemModel();
    _entriesModel    = new QStringListModel();
    _entriesModel->setStringList(QStringList());
}

DictManager::~DictManager()
{
    foreach (IDictionary* dictionary, _dictionaries)
        dictionary->release();
    clearDictionary();
    clearHistory();
    delete _dictionaryModel;
    delete _historyModel;
    delete _entriesModel;
}

IDictManager& IDictManager::instance()
{
    static DictManager manager;
    return manager;
}

int DictManager::findValidDictionaryVersion()
{
    foreach (int version, _dictionaries.keys())
        if (_dictionaries[version]->isValid())
            return version;

    return YdpTypes::InvalidVersion;
}

void DictManager::init()
{
    IConfigManager& confMan = IConfigManager::instance();

    if (confMan.config().dictionaryToOpen() == Recent)
    {
        _version   = YdpTypes::typeToVersion(confMan.config().recentDictionary());
        _direction = YdpTypes::typeToDirection(confMan.config().recentDictionary());
    }
    else
    {
        _version   = YdpTypes::typeToVersion(confMan.config().selectedDictionary());
        _direction = YdpTypes::typeToDirection(confMan.config().selectedDictionary());
    }

    if (YdpTypes::InvalidVersion == _version || !_dictionaries[_version]->isValid())
        _version = findValidDictionaryVersion();
}

bool DictManager::isCurrentDictValid() const
{
    return _version != YdpTypes::InvalidVersion;
}

void DictManager::initHistory()
{
    IConfigManager& confMan = IConfigManager::instance();

    foreach (HistoryItem historyItem, confMan.config().history())
    {
        YdpTypes::YdpDirection direction = YdpTypes::typeToDirection(historyItem.type());
        YdpTypes::YdpVersion   version   = YdpTypes::typeToVersion(historyItem.type());
        IDictionary*           dict      = _dictionaries[version];

        if (!dict->isValid())
            continue;

        if (_historyModel->rowCount() == confMan.config().maxNumberOfHistoryItems())
            break;

        QStandardItem *item = new QStandardItem();
        item->setText(historyItem.text());
        item->setIcon(QIcon(DictInstance::dictInst(version).iconName(direction)));
        item->setData(QVariant(historyItem.type()), Qt::UserRole);
        _historyModel->appendRow(item);
    }
}

void DictManager::insertHistoryItem(QString text)
{
    QStandardItem *item = new QStandardItem();
    item->setText(text);
    item->setIcon(QIcon(DictInstance::dictInst(_version).iconName(_direction)));
    item->setData(QVariant(_version + _direction), Qt::UserRole);
    _historyModel->insertRow(0, item);
    const int rowCount = _historyModel->rowCount();
    if (rowCount > IConfigManager::instance().config().maxNumberOfHistoryItems())
    {
        delete _historyModel->item(rowCount-1);
        _historyModel->removeRow(rowCount-1);
    }
    IConfigManager::instance().config().insertHistoryItem(text, _version + _direction);
}

void DictManager::initDictionary()
{
    foreach (int version, _dictionaries.keys())
    {
        if (_dictionaries[version]->isValid())
        {
            QStandardItem *item = new QStandardItem();
            DictInst instance = DictInstance::dictInst(version);
            item->setText(instance.description());
            item->setIcon(QIcon(instance.iconName(YdpTypes::Foreign)));
            item->setData(QVariant(version), Qt::UserRole);
            _dictionaryModel->appendRow(item);
        }
    }
}

void DictManager::initEntries()
{
    _entriesModel->setStringList(entries());
    emit dictionaryChanged(_version + _direction);
}

void DictManager::clearEntries()
{
    _entriesModel->setStringList(QStringList());
}

void DictManager::clearDictionary()
{
    const int itemCount = _dictionaryModel->rowCount();
    for (int index = 0; index < itemCount; index++)
        delete _dictionaryModel->item(index);
    _dictionaryModel->removeRows(0, itemCount);
}

void DictManager::clearHistory()
{
    const int itemCount = _historyModel->rowCount();
    for (int index = 0; index < itemCount; index++)
        delete _historyModel->item(index);
    _historyModel->removeRows(0, itemCount);
}

const QStringList& DictManager::entries() const
{
    return _dictionaries[_version]->entries(_direction);
}

QString DictManager::description(int index) const
{
    return _dictionaries[_version]->description(_direction, index);
}

int DictManager::entryToIndex(QString entry) const
{
    return _dictionaries[_version]->entryToIndex(_direction, entry);
}

QString DictManager::entriesText(int index) const
{
    return _entriesModel->data(_entriesModel->index(index), Qt::EditRole).toString();
}

QString DictManager::foreignIconName() const
{
    return DictInstance::dictInst(_version).foreignIconName();
}

QString DictManager::tooltip() const
{
    return DictInstance::dictInst(_version).toolTip();
}

QString DictManager::wavFormat() const
{
    return DictInstance::dictInst(_version).wavFormat();
}

void DictManager::invertDirection()
{
    _direction = YdpTypes::getOppositeDirection(_direction);
    initEntries();
}

bool DictManager::isDirectionForeign() const
{
    return YdpTypes::Foreign == _direction;
}

QString DictManager::samplesPath() const
{
    return IConfigManager::instance().config().samplesPath(_version);
}

void DictManager::setVersion(int dictionaryIndex)
{
    _version = _dictionaryModel->itemData(_dictionaryModel->index(dictionaryIndex, 0))[Qt::UserRole].toInt();
    initEntries();
}

bool DictManager::setType(int historyItemIndex)
{
    const int type = _historyModel->itemData(_historyModel->index(historyItemIndex, 0))[Qt::UserRole].toInt();
    const int version = YdpTypes::typeToVersion(type);
    const int direction = YdpTypes::typeToDirection(type);
    const bool versionChange = (_version != version);
    const bool directionChange = (_direction != direction);
    _version = version;
    _direction = direction;
    if (versionChange || directionChange)
        initEntries();
    return versionChange;
}

int DictManager::currentDictionaryIndex() const
{
    const int count = _dictionaryModel->rowCount();
    int index;
    for (index = 0; index < count; index++)
        if (_version == _dictionaryModel->itemData(_dictionaryModel->index(index, 0))[Qt::UserRole].toInt())
            break;

    return index;
}
