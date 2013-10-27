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

#include "config.h"
#include "ydptypes.h"

Config::Config()
{
    _dictionaryToOpen = Recent;
    _recentDictionary = YdpTypes::CollinsForeign;
    _selectedDictionary = YdpTypes::CollinsForeign;
    _maxNumberOfHistoryItems = 10;
    _storeHistory = 1;
    _clipboard = 0;
    _window = 0;
    _selfSelection = 0;
    _alwaysOnTop = 0;
    _autoShowWindow = 0;
    _geometry = QRect(100, 100, 440, 280);
    _sizes = QPair<int, int>(139, 278);
}

void Config::insertHistoryItem(QString text, int type)
{
    _history.insert(0, HistoryItem(text, type));
    const int count = _history.count();
    if (count > _maxNumberOfHistoryItems)
        _history.removeAt(count - 1);
}

void Config::appendHistoryItem(QString text, int type)
{
    _history.append(HistoryItem(text, type));
    const int count = _history.count();
    if (count > _maxNumberOfHistoryItems)
        _history.removeAt(0);
}
