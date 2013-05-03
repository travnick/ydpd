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

#include "dictinstance.h"
#include "ydptypes.h"

DictInst::DictInst(QString description, QString toolTip, QString foreignIconName, QString nativeIconName, QString wavFormat)
    : m_description(description),
      m_toolTip(toolTip),
      m_wavFormat(wavFormat)
{
    m_iconName[YdpTypes::Foreign] = foreignIconName;
    m_iconName[YdpTypes::Native]  = nativeIconName;
}

QString DictInst::foreignIconName() const
{
    return m_iconName[YdpTypes::Foreign];
}

QString DictInst::nativeIconName() const
{
    return m_iconName[YdpTypes::Native];
}

const VersionToDictInstMap DictInstance::m_instances = DictInstance::mapInit();

VersionToDictInstMap DictInstance::mapInit()
{
    VersionToDictInstMap    dictInstMap;

    dictInstMap[YdpTypes::Collins]       = DictInst("Collins",
                                                    QString::fromUtf8("Przełącz na angielski"),
                                                    ":/res/en.png",
                                                    ":/res/pl.png",
                                                    "/S%1/%2.WAV");

    dictInstMap[YdpTypes::Langenscheidt] = DictInst("Langenscheidt",
                                                     QString::fromUtf8("Przełącz na niemiecki"),
                                                     ":/res/de.png",
                                                     ":/res/pl.png",
                                                     "/s%1/%2.wav");

    //Here add new dictionary

    return dictInstMap;
}
