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

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QRect>
#include <QList>
#include <QPair>
#include <QMap>

class HistoryItem
{
public:
    HistoryItem(QString text, int type) : _text(text), _type(type) {}
    QString text() const { return _text; }
    int type() const { return _type; }
private:
    HistoryItem() {}
    QString _text;
    int     _type;
};

class Paths
{
public:
    Paths() {}
    void setDictionary(QString dictionary) { _dictionary = dictionary; }
    QString dictionary() const { return _dictionary; }
    void setSamples(QString samples) { _samples = samples; }
    QString samples() const { return _samples; }
private:
    QString _dictionary;
    QString _samples;
};

typedef QList<HistoryItem> HistoryList;
typedef QMap<int, Paths> VersionToPathMap;
typedef QList<int> VersionList;

class Config
{
public:
    Config();
    void insertHistoryItem(QString text, int type);
    void appendHistoryItem(QString text, int type);
    void setDictionaryPath(int version, QString path) { _paths[version].setDictionary(path); }
    QString dictionaryPath(int version) const { return _paths[version].dictionary(); }
    void setSamplesPath(int version, QString path) { _paths[version].setSamples(path); }
    QString samplesPath(int version) const { return _paths[version].samples(); }
    VersionList versionList() const { return _paths.keys(); }
    void setDictionaryToOpen(int value) { _dictionaryToOpen = value; }
    int dictionaryToOpen() const { return _dictionaryToOpen; }
    void setRecentDictionary(int version) { _recentDictionary = version; }
    int recentDictionary() const { return _recentDictionary; }
    void setSelectedDictionary(int version) { _selectedDictionary = version; }
    int selectedDictionary() const { return _selectedDictionary; }
    void setMaxNumberOfHistoryItems(int value) { _maxNumberOfHistoryItems = value; }
    int maxNumberOfHistoryItems() const { return _maxNumberOfHistoryItems; }
    void setStoreHistory(int value) { _storeHistory = value; }
    int storeHistory() const { return _storeHistory; }
    const HistoryList& history() const { return _history; }
    void setGeometry(QRect rect) { _geometry = rect; }
    QRect geometry() const { return _geometry; }
    void setSplitters(QPair<int, int> sizes) { _sizes = sizes; }
    QPair<int, int> getSplitters() const { return _sizes; }
    void setClipboard(bool enabled) { _clipboard = enabled; }
    bool clipboard() const { return _clipboard; }
    void setSelfSelection(bool enabled) { _selfSelection = enabled; }
    bool selfSelection() const { return _selfSelection; }
    void setAlwaysOnTop(bool enabled) { _alwaysOnTop = enabled; }
    bool alwaysOnTop() const { return _alwaysOnTop; }
    void setAutoShowWindow(bool enabled) { _autoShowWindow = enabled; }
    bool autoShowWindow() const { return _autoShowWindow; }
private:
    VersionToPathMap _paths;
    int _dictionaryToOpen;
    int _recentDictionary;
    int _selectedDictionary;
    int _maxNumberOfHistoryItems;
    int _storeHistory;
    int _clipboard;
    int _window;
    int _selfSelection;
    int _alwaysOnTop;
    int _autoShowWindow;
    HistoryList _history;
    QRect _geometry;
    QPair<int, int> _sizes;
};

#endif // CONFIG_H
