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

#ifndef DICTINSTANCE_H
#define DICTINSTANCE_H

#include <QString>
#include <QList>
#include <QMap>

class DictInst
{
public:
    DictInst(QString description, QString toolTip, QString foreignIconName, QString nativeIconName, QString wavFormat);
    DictInst() {}
    QString description() const { return _description; }
    QString toolTip() const { return _toolTip; }
    QString iconName(int direction) const { return _iconName[direction]; }
    QString foreignIconName() const;
    QString nativeIconName() const;
    QString wavFormat() const { return _wavFormat; }
private:
    QString _description;
    QString _toolTip;
    QString _iconName[2];
    QString _wavFormat;
};

typedef QMap<int, DictInst> VersionToDictInstMap;
typedef QList<int> VersionList;

class DictInstance
{
public:
    static const DictInst dictInst(int version)
    {
        return _instances[version];
    }
    static const VersionList versionList()
    {
        return _instances.keys();
    }

private:
    DictInstance() {}
    static const VersionToDictInstMap _instances;
    static VersionToDictInstMap mapInit();
};

#endif // DICTINSTANCE_H
