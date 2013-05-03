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

#ifndef DICTMANAGER_H
#define DICTMANAGER_H

#include "QAbstractItemModel"

class IDictManager : public QObject
{
    Q_OBJECT
signals:
    void dictionaryChanged(int type);
public:
    static IDictManager& instance();
    virtual QAbstractItemModel* entriesModel() const = 0;
    virtual QAbstractItemModel* dictionaryModel() const = 0;
    virtual QAbstractItemModel* historyModel() const = 0;
    virtual bool isTypeEqual(int type) = 0;
    virtual int currentDictionaryIndex() const = 0;
    virtual bool isCurrentDictValid() const = 0;
    virtual void setVersion(int currentDictionaryIndex) = 0;
    virtual void invertDirection() = 0;
    virtual bool setType(int historyItemIndex) = 0;
    virtual bool isDirectionForeign() const = 0;
    virtual void initHistory() = 0;
    virtual void insertHistoryItem(QString text) = 0;
    virtual void initDictionary() = 0;
    virtual void clearEntries() = 0;
    virtual void clearDictionary() = 0;
    virtual void clearHistory() = 0;
    virtual void initEntries() = 0;
    virtual void init() = 0;
    virtual const QStringList& entries() const = 0;
    virtual QString description(int index) const = 0;
    virtual int entryToIndex(QString entry) const = 0;
    virtual QString entriesText(int index) const = 0;
    virtual QString foreignIconName() const = 0;
    virtual QString tooltip() const = 0;
    virtual QString wavFormat() const = 0;
    virtual QString samplesPath() const = 0;
protected:
    IDictManager() {}
    virtual ~IDictManager() {}
};

#endif // DICTMANAGER_H
