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

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include "config.h"

class IConfigManager : public QObject
{
    Q_OBJECT
public:
    static IConfigManager& instance();
    virtual bool save() const = 0;
    virtual bool load() = 0;
    virtual Config& config() = 0;
    void setConfig(const Config& config);
signals:
    void dictionaryDirectoryChanged(int version, QString dictionaryPath);
public slots:
    virtual void onClipboardChanged(bool clipboard) = 0;
    virtual void onSelfSelectionChanged(bool selfSelection) = 0;
    virtual void onAlwaysOnTopChanged(bool alwaysOnTop) = 0;
    virtual void onAutoShowWindowChanged(bool autoShowWindow) = 0;
protected:
    IConfigManager() {}
    virtual ~IConfigManager() {}
    Config _config;
};

#endif // CONFIGMANAGER_H
