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

#pragma once

#include <QString>
#include <QStringList>

class IDictionary : public QObject
{
    Q_OBJECT
public:
    static IDictionary* createInstance(int version, QString dictionaryPath);
    virtual void release() = 0;
    virtual bool isValid() = 0;
    virtual const QStringList& entries(int direction) = 0;
    virtual QString description(int direction, int index) = 0;
    virtual int entryToIndex(int direction, QString entry) = 0;
public slots:
    virtual void initialize(int version, QString dictionaryPath) = 0;
protected:
    IDictionary() {}
    virtual ~IDictionary() {}
};
