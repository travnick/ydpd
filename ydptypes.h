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

enum DictionaryToOpen
{
    Recent   = 0,
    Selected = 1
};

class YdpTypes
{
public:
    enum YdpVersion
    {
        InvalidVersion = 0,
        Collins        = 100,
        Langenscheidt  = 200
        //Here add new dictionary
    };

    enum YdpDirection
    {
        Foreign = 0,
        Native  = 1
    };

    enum YdpType
    {
        InvalidType          = 0,
        CollinsForeign       = Collins + Foreign,
        CollinsNative        = Collins + Native,
        LangenscheidtForeign = Langenscheidt + Foreign,
        LangenscheidtNative  = Langenscheidt + Native
        //Here add new dictionary
    };

    static YdpDirection typeToDirection(int type)
    {
        return static_cast<YdpDirection>(type % 100);
    }

    static YdpVersion typeToVersion(int type)
    {
        return static_cast<YdpVersion>(type - typeToDirection(type));
    }

    static YdpDirection getOppositeDirection(int direction)
    {
        return static_cast<YdpDirection>((direction + 1) % 2);
    }

private:
    YdpTypes() {}
};
