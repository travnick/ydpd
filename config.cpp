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
    m_dictionaryToOpen = Recent;
    m_recentDictionary = YdpTypes::CollinsForeign;
    m_selectedDictionary = YdpTypes::CollinsForeign;
    m_maxNumberOfHistoryItems = 10;
    m_storeHistory = 1;
    m_clipboard = 0;
    m_window = 0;
    m_selfSelection = 0;
    m_alwaysOnTop = 0;
    m_autoShowWindow = 0;
    m_geometry = QRect(100, 100, 440, 280);
    m_sizes = QPair<int, int>(139, 278);
}

void Config::insertHistoryItem(QString text, int type)
{
    m_history.insert(0, HistoryItem(text, type));
    const int count = m_history.count();
    if (count > m_maxNumberOfHistoryItems)
        m_history.removeAt(count - 1);
}

void Config::appendHistoryItem(QString text, int type)
{
    m_history.append(HistoryItem(text, type));
    const int count = m_history.count();
    if (count > m_maxNumberOfHistoryItems)
        m_history.removeAt(0);
}
