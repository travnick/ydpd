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
    HistoryItem(QString text, int type) : m_text(text), m_type(type) {}
    QString text() const { return m_text; }
    int type() const { return m_type; }
private:
    HistoryItem() {}
    QString m_text;
    int     m_type;
};

class Paths
{
public:
    Paths() {}
    void setDictionary(QString dictionary) { m_dictionary = dictionary; }
    QString dictionary() const { return m_dictionary; }
    void setSamples(QString samples) { m_samples = samples; }
    QString samples() const { return m_samples; }
private:
    QString m_dictionary;
    QString m_samples;
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
    void setDictionaryPath(int version, QString path) { m_paths[version].setDictionary(path); }
    QString dictionaryPath(int version) const { return m_paths[version].dictionary(); }
    void setSamplesPath(int version, QString path) { m_paths[version].setSamples(path); }
    QString samplesPath(int version) const { return m_paths[version].samples(); }
    VersionList versionList() const { return m_paths.keys(); }
    void setDictionaryToOpen(int value) { m_dictionaryToOpen = value; }
    int dictionaryToOpen() const { return m_dictionaryToOpen; }
    void setRecentDictionary(int version) { m_recentDictionary = version; }
    int recentDictionary() const { return m_recentDictionary; }
    void setSelectedDictionary(int version) { m_selectedDictionary = version; }
    int selectedDictionary() const { return m_selectedDictionary; }
    void setMaxNumberOfHistoryItems(int value) { m_maxNumberOfHistoryItems = value; }
    int maxNumberOfHistoryItems() const { return m_maxNumberOfHistoryItems; }
    void setStoreHistory(int value) { m_storeHistory = value; }
    int storeHistory() const { return m_storeHistory; }
    const HistoryList& history() const { return m_history; }
    void setGeometry(QRect rect) { m_geometry = rect; }
    QRect geometry() const { return m_geometry; }
    void setSplitters(QPair<int, int> sizes) { m_sizes = sizes; }
    QPair<int, int> getSplitters() const { return m_sizes; }
    void setClipboard(bool enabled) { m_clipboard = enabled; }
    bool clipboard() const { return m_clipboard; }
    void setSelfSelection(bool enabled) { m_selfSelection = enabled; }
    bool selfSelection() const { return m_selfSelection; }
    void setAlwaysOnTop(bool enabled) { m_alwaysOnTop = enabled; }
    bool alwaysOnTop() const { return m_alwaysOnTop; }
    void setAutoShowWindow(bool enabled) { m_autoShowWindow = enabled; }
    bool autoShowWindow() const { return m_autoShowWindow; }
private:
    VersionToPathMap m_paths;
    int m_dictionaryToOpen;
    int m_recentDictionary;
    int m_selectedDictionary;
    int m_maxNumberOfHistoryItems;
    int m_storeHistory;
    int m_clipboard;
    int m_window;
    int m_selfSelection;
    int m_alwaysOnTop;
    int m_autoShowWindow;
    HistoryList m_history;
    QRect m_geometry;
    QPair<int, int> m_sizes;
};

#endif // CONFIG_H
