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
    Config& getConfig();
    void setConfig(const Config& config);
signals:
    void dictionaryDirectoryChanged(int version, QString dictionaryPath);
public slots:
    void onClipboardChanged(bool clipboard) { _config.setClipboard(clipboard); }
    void onSelfSelectionChanged(bool selfSelection) { _config.setSelfSelection(selfSelection); }
    void onAlwaysOnTopChanged(bool alwaysOnTop) { _config.setAlwaysOnTop(alwaysOnTop); }
    void onAutoShowWindowChanged(bool autoShowWindow) { _config.setAutoShowWindow(autoShowWindow); }
private:
    static bool loadStartup(QDomElement& root, Config& config);
    static bool loadHistory(QDomElement& root, Config& config);
    static bool loadDictionary(QDomElement& root, Config& config);
    static bool loadOptions(QDomElement& root, Config& config);
    static bool loadGeometry(QDomElement& root, Config& config);
    static bool loadSplitters(QDomElement& root, Config& config);
    const static QString _configFileName;
    const static QString _mainTagName;
    const static QString _startupTagName;
    const static QString _dictToOpenAttribName;
    const static QString _recentDictAttribName;
    const static QString _selectedDictAttribName;
    const static QString _historyTagName;
    const static QString _historyEnabledAttribName;
    const static QString _maxHistoryItemsAttribName;
    const static QString _historyItemTagName;
    const static QString _historyItemTypeAttribName;
    const static QString _historyItemTextAttribName;
    const static QString _dictTagName;
    const static QString _dictVersionAttribName;
    const static QString _dictDataPathAttribName;
    const static QString _dictSamplesPathAttribName;
    const static QString _optionsTagName;
    const static QString _clipboardAttribName;
    const static QString _selfSelectionAttribName;
    const static QString _alwaysOnTopAttribName;
    const static QString _autoShowWindowAttribName;
    const static QString _geometryTagName;
    const static QString _geometryXAttribName;
    const static QString _geometryYAttribName;
    const static QString _geometryWidthAttribName;
    const static QString _geometryHeightAttribName;
    const static QString _splittersTagName;
    const static QString _leftSplitterAttribName;
    const static QString _rightSplitterAttribName;
};

const QString ConfigManager::_configFileName = ".ydpd.xml";
const QString ConfigManager::_mainTagName = "YdpdConfig";
const QString ConfigManager::_startupTagName = "Startup";
const QString ConfigManager::_dictToOpenAttribName = "open";
const QString ConfigManager::_recentDictAttribName = "recent";
const QString ConfigManager::_selectedDictAttribName = "selected";
const QString ConfigManager::_historyTagName = "History";
const QString ConfigManager::_historyEnabledAttribName = "enabled";
const QString ConfigManager::_maxHistoryItemsAttribName = "maxItems";
const QString ConfigManager::_historyItemTagName = "Item";
const QString ConfigManager::_historyItemTypeAttribName = "type";
const QString ConfigManager::_historyItemTextAttribName = "text";
const QString ConfigManager::_dictTagName = "Dictionary";
const QString ConfigManager::_dictVersionAttribName = "version";
const QString ConfigManager::_dictDataPathAttribName = "data";
const QString ConfigManager::_dictSamplesPathAttribName = "samples";
const QString ConfigManager::_optionsTagName = "Options";
const QString ConfigManager::_clipboardAttribName = "clipboard";
const QString ConfigManager::_selfSelectionAttribName = "selfSelection";
const QString ConfigManager::_alwaysOnTopAttribName = "alwaysOnTop";
const QString ConfigManager::_autoShowWindowAttribName = "autoShowWindow";
const QString ConfigManager::_geometryTagName = "Geometry";
const QString ConfigManager::_geometryXAttribName = "x";
const QString ConfigManager::_geometryYAttribName = "y";
const QString ConfigManager::_geometryWidthAttribName = "width";
const QString ConfigManager::_geometryHeightAttribName = "height";
const QString ConfigManager::_splittersTagName = "Sizes";
const QString ConfigManager::_leftSplitterAttribName = "a";
const QString ConfigManager::_rightSplitterAttribName = "b";

ConfigManager::ConfigManager()
{
#ifdef DEBUG
    bool value = load();
    Q_ASSERT(value);
#else
    load();
#endif
}

ConfigManager::~ConfigManager()
{
    save();
}

Config& ConfigManager::getConfig()
{
    return _config;
}

bool ConfigManager::loadStartup(QDomElement& root, Config& config)
{
    QDomNodeList list = root.elementsByTagName(_startupTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(_dictToOpenAttribName);
    if (attribute.isNull())
        return false;
    config.setDictionaryToOpen(attribute.value().toInt());
    attribute = element.attributeNode(_recentDictAttribName);
    if (attribute.isNull())
        return false;
    config.setRecentDictionary(attribute.value().toInt());
    attribute = element.attributeNode(_selectedDictAttribName);
    if (attribute.isNull())
        return false;
    config.setSelectedDictionary(attribute.value().toInt());

    return true;
}

bool ConfigManager::loadHistory(QDomElement& root, Config& config)
{
    QDomNodeList list = root.elementsByTagName(_historyTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(_historyEnabledAttribName);
    if (attribute.isNull())
        return false;
    config.setStoreHistory(attribute.value().toInt());
    attribute = element.attributeNode(_maxHistoryItemsAttribName);
    if (attribute.isNull())
        return false;
    config.setMaxNumberOfHistoryItems(attribute.value().toInt());
    list = element.elementsByTagName(_historyItemTagName);
    uint maxNumberOfHistoryItems = config.maxNumberOfHistoryItems();
    for (uint index = 0; index < list.length() && index < maxNumberOfHistoryItems; index++)
    {
        element = list.item(index).toElement();
        if (!element.hasAttribute(_historyItemTypeAttribName) || !element.hasAttribute(_historyItemTextAttribName))
            return false;
        config.appendHistoryItem(element.attribute(_historyItemTextAttribName), element.attribute(_historyItemTypeAttribName).toInt());
    }

    return true;
}

bool ConfigManager::loadDictionary(QDomElement& root, Config& config)
{
    QDomNodeList list = root.elementsByTagName(_dictTagName);
    for (uint index = 0; index < list.length(); index++)
    {
        QDomElement element = list.item(index).toElement();
        if (!element.hasAttribute(_dictVersionAttribName) || !element.hasAttribute(_dictDataPathAttribName) || !element.hasAttribute(_dictSamplesPathAttribName))
            return false;
        config.setDictionaryPath(element.attribute(_dictVersionAttribName).toInt(), element.attribute(_dictDataPathAttribName));
        config.setSamplesPath(element.attribute(_dictVersionAttribName).toInt(), element.attribute(_dictSamplesPathAttribName));
    }

    return true;
}

bool ConfigManager::loadOptions(QDomElement& root, Config& config)
{
    QDomNodeList list = root.elementsByTagName(_optionsTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(_clipboardAttribName);
    if (attribute.isNull())
        return false;
    config.setClipboard(attribute.value().toInt());
    attribute = element.attributeNode(_selfSelectionAttribName);
    if (attribute.isNull())
        return false;
    config.setSelfSelection(attribute.value().toInt());
    attribute = element.attributeNode(_alwaysOnTopAttribName);
    if (attribute.isNull())
        return false;
    config.setAlwaysOnTop(attribute.value().toInt());
    attribute = element.attributeNode(_autoShowWindowAttribName);
    if (attribute.isNull())
        return false;
    config.setAutoShowWindow(attribute.value().toInt());

    return true;
}

bool ConfigManager::loadGeometry(QDomElement& root, Config& config)
{
    QRect geometry;
    QDomNodeList list = root.elementsByTagName(_geometryTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(_geometryXAttribName);
    if (attribute.isNull())
        return false;
    geometry.setX(attribute.value().toInt());
    attribute = element.attributeNode(_geometryYAttribName);
    if (attribute.isNull())
        return false;
    geometry.setY(attribute.value().toInt());
    attribute = element.attributeNode(_geometryHeightAttribName);
    if (attribute.isNull())
        return false;
    geometry.setHeight(attribute.value().toInt());
    attribute = element.attributeNode(_geometryWidthAttribName);
    if (attribute.isNull())
        return false;
    geometry.setWidth(attribute.value().toInt());
    config.setGeometry(geometry);

    return true;
}

bool ConfigManager::loadSplitters(QDomElement& root, Config& config)
{
    QPair<int, int> sizes;
    QDomNodeList list = root.elementsByTagName(_splittersTagName);
    if (list.length() != 1)
        return false;
    QDomElement element = list.item(0).toElement();
    QDomAttr attribute = element.attributeNode(_leftSplitterAttribName);
    if (attribute.isNull())
        return false;
    sizes.first = attribute.value().toInt();
    attribute = element.attributeNode(_rightSplitterAttribName);
    if (attribute.isNull())
        return false;
    sizes.second = attribute.value().toInt();
    config.setSplitters(sizes);

    return true;
}

bool ConfigManager::load()
{
    QDomDocument config;
    QFile configFile(QDir::homePath()+"/"+_configFileName);
    if (!configFile.open(QIODevice::ReadOnly))
        return false;
    if (!config.setContent(&configFile))
    {
        configFile.close();
        return false;
    }
    configFile.close();

    QDomElement root = config.documentElement();
    if (root.tagName() != _mainTagName)
        return false;

    if (!loadStartup(root, _config))
        return false;

    if (!loadHistory(root, _config))
        return false;

    if (!loadDictionary(root, _config))
        return false;

    if (!loadOptions(root, _config))
        return false;

    if (!loadGeometry(root, _config))
        return false;

    if (!loadSplitters(root, _config))
        return false;

    return true;
}

bool ConfigManager::save() const
{
    QDomDocument config;

    QDomNode node(config.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
    config.insertBefore(node, config.firstChild());

    QDomElement root = config.createElement(_mainTagName);
    config.appendChild(root);

    QDomElement element = config.createElement(_startupTagName);
    element.setAttribute(_dictToOpenAttribName, _config.dictionaryToOpen());
    element.setAttribute(_recentDictAttribName, _config.recentDictionary());
    element.setAttribute(_selectedDictAttribName, _config.selectedDictionary());
    root.appendChild(element);

    element = config.createElement(_historyTagName);
    element.setAttribute(_historyEnabledAttribName, _config.storeHistory());
    element.setAttribute(_maxHistoryItemsAttribName, _config.maxNumberOfHistoryItems());
    for (int index = 0; index < _config.history().length() && index < _config.maxNumberOfHistoryItems() && _config.storeHistory(); index++)
    {
        QDomElement subElement = config.createElement(_historyItemTagName);
        subElement.setAttribute(_historyItemTypeAttribName, _config.history().at(index).getType());
        subElement.setAttribute(_historyItemTextAttribName, _config.history().at(index).getText());
        element.appendChild(subElement);
    }
    root.appendChild(element);

    foreach (int version, _config.versionList())
    {
        element = config.createElement(_dictTagName);
        element.setAttribute(_dictVersionAttribName, version);
        element.setAttribute(_dictDataPathAttribName, _config.getDictionaryPath(version));
        element.setAttribute(_dictSamplesPathAttribName, _config.samplesPath(version));
        root.appendChild(element);
    }

    element = config.createElement(_optionsTagName);
    element.setAttribute(_clipboardAttribName, _config.clipboard());
    element.setAttribute(_selfSelectionAttribName, _config.selfSelection());
    element.setAttribute(_alwaysOnTopAttribName, _config.alwaysOnTop());
    element.setAttribute(_autoShowWindowAttribName, _config.autoShowWindow());
    root.appendChild(element);

    element = config.createElement(_geometryTagName);
    element.setAttribute(_geometryXAttribName, _config.geometry().x());
    element.setAttribute(_geometryYAttribName, _config.geometry().y());
    element.setAttribute(_geometryWidthAttribName, _config.geometry().width());
    element.setAttribute(_geometryHeightAttribName, _config.geometry().height());
    root.appendChild(element);

    element = config.createElement(_splittersTagName);
    element.setAttribute(_leftSplitterAttribName, _config.getSplitters().first);
    element.setAttribute(_rightSplitterAttribName, _config.getSplitters().second);
    root.appendChild(element);

    QFile configFile(QDir::homePath()+"/"+_configFileName);
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
        emit dictionaryDirectoryChanged(version, config.getDictionaryPath(version));

    _config = config;
}
