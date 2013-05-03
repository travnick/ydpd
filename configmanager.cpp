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

#include "configmanager.h"
#include <QtXml>

class ConfigManager : public IConfigManager
{
public:
    ConfigManager();
    ~ConfigManager();
    bool save() const;
    bool load();
    Config& config();
    void setConfig(const Config& config);
signals:
    void dictionaryDirectoryChanged(int version, QString dictionaryPath);
public slots:
    void onClipboardChanged(bool clipboard) { m_config.setClipboard(clipboard); }
    void onSelfSelectionChanged(bool selfSelection) { m_config.setSelfSelection(selfSelection); }
    void onAlwaysOnTopChanged(bool alwaysOnTop) { m_config.setAlwaysOnTop(alwaysOnTop); }
    void onAutoShowWindowChanged(bool autoShowWindow) { m_config.setAutoShowWindow(autoShowWindow); }
private:
    static bool loadStartup(QDomElement& root, Config& config);
    static bool loadHistory(QDomElement& root, Config& config);
    static bool loadDictionary(QDomElement& root, Config& config);
    static bool loadOptions(QDomElement& root, Config& config);
    static bool loadGeometry(QDomElement& root, Config& config);
    static bool loadSplitters(QDomElement& root, Config& config);
    const static QString m_configFileName;
    const static QString m_mainTagName;
    const static QString m_startupTagName;
    const static QString m_dictToOpenAttribName;
    const static QString m_recentDictAttribName;
    const static QString m_selectedDictAttribName;
    const static QString m_historyTagName;
    const static QString m_historyEnabledAttribName;
    const static QString m_maxHistoryItemsAttribName;
    const static QString m_historyItemTagName;
    const static QString m_historyItemTypeAttribName;
    const static QString m_historyItemTextAttribName;
    const static QString m_dictTagName;
    const static QString m_dictVersionAttribName;
    const static QString m_dictDataPathAttribName;
    const static QString m_dictSamplesPathAttribName;
    const static QString m_optionsTagName;
    const static QString m_clipboardAttribName;
    const static QString m_selfSelectionAttribName;
    const static QString m_alwaysOnTopAttribName;
    const static QString m_autoShowWindowAttribName;
    const static QString m_geometryTagName;
    const static QString m_geometryXAttribName;
    const static QString m_geometryYAttribName;
    const static QString m_geometryWidthAttribName;
    const static QString m_geometryHeightAttribName;
    const static QString m_splittersTagName;
    const static QString m_leftSplitterAttribName;
    const static QString m_rightSplitterAttribName;
};

const QString ConfigManager::m_configFileName = ".ydpd.xml";
const QString ConfigManager::m_mainTagName = "YdpdConfig";
const QString ConfigManager::m_startupTagName = "Startup";
const QString ConfigManager::m_dictToOpenAttribName = "open";
const QString ConfigManager::m_recentDictAttribName = "recent";
const QString ConfigManager::m_selectedDictAttribName = "selected";
const QString ConfigManager::m_historyTagName = "History";
const QString ConfigManager::m_historyEnabledAttribName = "enabled";
const QString ConfigManager::m_maxHistoryItemsAttribName = "maxItems";
const QString ConfigManager::m_historyItemTagName = "Item";
const QString ConfigManager::m_historyItemTypeAttribName = "type";
const QString ConfigManager::m_historyItemTextAttribName = "text";
const QString ConfigManager::m_dictTagName = "Dictionary";
const QString ConfigManager::m_dictVersionAttribName = "version";
const QString ConfigManager::m_dictDataPathAttribName = "data";
const QString ConfigManager::m_dictSamplesPathAttribName = "samples";
const QString ConfigManager::m_optionsTagName = "Options";
const QString ConfigManager::m_clipboardAttribName = "clipboard";
const QString ConfigManager::m_selfSelectionAttribName = "selfSelection";
const QString ConfigManager::m_alwaysOnTopAttribName = "alwaysOnTop";
const QString ConfigManager::m_autoShowWindowAttribName = "autoShowWindow";
const QString ConfigManager::m_geometryTagName = "Geometry";
const QString ConfigManager::m_geometryXAttribName = "x";
const QString ConfigManager::m_geometryYAttribName = "y";
const QString ConfigManager::m_geometryWidthAttribName = "width";
const QString ConfigManager::m_geometryHeightAttribName = "height";
const QString ConfigManager::m_splittersTagName = "Sizes";
const QString ConfigManager::m_leftSplitterAttribName = "a";
const QString ConfigManager::m_rightSplitterAttribName = "b";

ConfigManager::ConfigManager()
{
#ifdef DEBUG
    bool value = load();
#else
    load();
#endif
    Q_ASSERT(value);
}

ConfigManager::~ConfigManager()
{
    save();
}

Config& ConfigManager::config()
{
    return m_config;
}

bool ConfigManager::loadStartup(QDomElement& root, Config& config)
{
    QDomNodeList list = root.elementsByTagName(m_startupTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(m_dictToOpenAttribName);
    if (attribute.isNull())
        return false;
    config.setDictionaryToOpen(attribute.value().toInt());
    attribute = element.attributeNode(m_recentDictAttribName);
    if (attribute.isNull())
        return false;
    config.setRecentDictionary(attribute.value().toInt());
    attribute = element.attributeNode(m_selectedDictAttribName);
    if (attribute.isNull())
        return false;
    config.setSelectedDictionary(attribute.value().toInt());

    return true;
}

bool ConfigManager::loadHistory(QDomElement& root, Config& config)
{
    QDomNodeList list = root.elementsByTagName(m_historyTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(m_historyEnabledAttribName);
    if (attribute.isNull())
        return false;
    config.setStoreHistory(attribute.value().toInt());
    attribute = element.attributeNode(m_maxHistoryItemsAttribName);
    if (attribute.isNull())
        return false;
    config.setMaxNumberOfHistoryItems(attribute.value().toInt());
    list = element.elementsByTagName(m_historyItemTagName);
    for (uint index = 0; index < list.length() && index < (uint)config.maxNumberOfHistoryItems(); index++)
    {
        element = list.item(index).toElement();
        if (!element.hasAttribute(m_historyItemTypeAttribName) || !element.hasAttribute(m_historyItemTextAttribName))
            return false;
        config.appendHistoryItem(element.attribute(m_historyItemTextAttribName), element.attribute(m_historyItemTypeAttribName).toInt());
    }

    return true;
}

bool ConfigManager::loadDictionary(QDomElement& root, Config& config)
{
    QDomNodeList list = root.elementsByTagName(m_dictTagName);
    for (uint index = 0; index < list.length(); index++)
    {
        QDomElement element = list.item(index).toElement();
        if (!element.hasAttribute(m_dictVersionAttribName) || !element.hasAttribute(m_dictDataPathAttribName) || !element.hasAttribute(m_dictSamplesPathAttribName))
            return false;
        config.setDictionaryPath(element.attribute(m_dictVersionAttribName).toInt(), element.attribute(m_dictDataPathAttribName));
        config.setSamplesPath(element.attribute(m_dictVersionAttribName).toInt(), element.attribute(m_dictSamplesPathAttribName));
    }

    return true;
}

bool ConfigManager::loadOptions(QDomElement& root, Config& config)
{
    QDomNodeList list = root.elementsByTagName(m_optionsTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(m_clipboardAttribName);
    if (attribute.isNull())
        return false;
    config.setClipboard(attribute.value().toInt());
    attribute = element.attributeNode(m_selfSelectionAttribName);
    if (attribute.isNull())
        return false;
    config.setSelfSelection(attribute.value().toInt());
    attribute = element.attributeNode(m_alwaysOnTopAttribName);
    if (attribute.isNull())
        return false;
    config.setAlwaysOnTop(attribute.value().toInt());
    attribute = element.attributeNode(m_autoShowWindowAttribName);
    if (attribute.isNull())
        return false;
    config.setAutoShowWindow(attribute.value().toInt());

    return true;
}

bool ConfigManager::loadGeometry(QDomElement& root, Config& config)
{
    QRect geometry;
    QDomNodeList list = root.elementsByTagName(m_geometryTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(m_geometryXAttribName);
    if (attribute.isNull())
        return false;
    geometry.setX(attribute.value().toInt());
    attribute = element.attributeNode(m_geometryYAttribName);
    if (attribute.isNull())
        return false;
    geometry.setY(attribute.value().toInt());
    attribute = element.attributeNode(m_geometryHeightAttribName);
    if (attribute.isNull())
        return false;
    geometry.setHeight(attribute.value().toInt());
    attribute = element.attributeNode(m_geometryWidthAttribName);
    if (attribute.isNull())
        return false;
    geometry.setWidth(attribute.value().toInt());
    config.setGeometry(geometry);

    return true;
}

bool ConfigManager::loadSplitters(QDomElement& root, Config& config)
{
    QPair<int, int> sizes;
    QDomNodeList list = root.elementsByTagName(m_splittersTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(m_leftSplitterAttribName);
    if (attribute.isNull())
        return false;
    sizes.first = attribute.value().toInt();
    attribute = element.attributeNode(m_rightSplitterAttribName);
    if (attribute.isNull())
        return false;
    sizes.second = attribute.value().toInt();
    config.setSplitters(sizes);

    return true;
}

bool ConfigManager::load()
{
    QDomDocument config;
    QFile configFile(QDir::homePath()+"/"+m_configFileName);
    if (!configFile.open(QIODevice::ReadOnly))
        return false;
    if (!config.setContent(&configFile))
    {
        configFile.close();
        return false;
    }
    configFile.close();

    QDomElement root = config.documentElement();
    if (root.tagName() != m_mainTagName)
        return false;

    if (!loadStartup(root, m_config))
        return false;

    if (!loadHistory(root, m_config))
        return false;

    if (!loadDictionary(root, m_config))
        return false;

    if (!loadOptions(root, m_config))
        return false;

    if (!loadGeometry(root, m_config))
        return false;

    if (!loadSplitters(root, m_config))
        return false;

    return true;
}

bool ConfigManager::save() const
{
    QDomDocument config;

    QDomNode node(config.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
    config.insertBefore(node, config.firstChild());

    QDomElement root = config.createElement(m_mainTagName);
    config.appendChild(root);

    QDomElement element = config.createElement(m_startupTagName);
    element.setAttribute(m_dictToOpenAttribName, m_config.dictionaryToOpen());
    element.setAttribute(m_recentDictAttribName, m_config.recentDictionary());
    element.setAttribute(m_selectedDictAttribName, m_config.selectedDictionary());
    root.appendChild(element);

    element = config.createElement(m_historyTagName);
    element.setAttribute(m_historyEnabledAttribName, m_config.storeHistory());
    element.setAttribute(m_maxHistoryItemsAttribName, m_config.maxNumberOfHistoryItems());
    for (int index = 0; index < m_config.history().length() && index < m_config.maxNumberOfHistoryItems() && m_config.storeHistory(); index++)
    {
        QDomElement subElement = config.createElement(m_historyItemTagName);
        subElement.setAttribute(m_historyItemTypeAttribName, m_config.history().at(index).type());
        subElement.setAttribute(m_historyItemTextAttribName, m_config.history().at(index).text());
        element.appendChild(subElement);
    }
    root.appendChild(element);

    foreach (int version, m_config.versionList())
    {
        element = config.createElement(m_dictTagName);
        element.setAttribute(m_dictVersionAttribName, version);
        element.setAttribute(m_dictDataPathAttribName, m_config.dictionaryPath(version));
        element.setAttribute(m_dictSamplesPathAttribName, m_config.samplesPath(version));
        root.appendChild(element);
    }

    element = config.createElement(m_optionsTagName);
    element.setAttribute(m_clipboardAttribName, m_config.clipboard());
    element.setAttribute(m_selfSelectionAttribName, m_config.selfSelection());
    element.setAttribute(m_alwaysOnTopAttribName, m_config.alwaysOnTop());
    element.setAttribute(m_autoShowWindowAttribName, m_config.autoShowWindow());
    root.appendChild(element);

    element = config.createElement(m_geometryTagName);
    element.setAttribute(m_geometryXAttribName, m_config.geometry().x());
    element.setAttribute(m_geometryYAttribName, m_config.geometry().y());
    element.setAttribute(m_geometryWidthAttribName, m_config.geometry().width());
    element.setAttribute(m_geometryHeightAttribName, m_config.geometry().height());
    root.appendChild(element);

    element = config.createElement(m_splittersTagName);
    element.setAttribute(m_leftSplitterAttribName, m_config.getSplitters().first);
    element.setAttribute(m_rightSplitterAttribName, m_config.getSplitters().second);
    root.appendChild(element);

    QFile configFile(QDir::homePath()+"/"+m_configFileName);
    if (!configFile.open(QIODevice::WriteOnly))
        return false;
    QTextStream textStream(&configFile);
    textStream.setCodec("UTF-8");
    textStream << config.toString();
    configFile.close();

    return true;
}

IConfigManager& IConfigManager::instance()
{
    static ConfigManager manager;
    return manager;
}

void IConfigManager::setConfig(const Config& config)
{
    foreach (int version, config.versionList())
        emit dictionaryDirectoryChanged(version, config.dictionaryPath(version));

    m_config = config;
}
