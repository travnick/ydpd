/***************************************************************************
 *   Copyright (C) 2012 by Marcin Nawrocki <mar.nawrocki@gmail.com>        *
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

#include "plasma-ydpd.h"
#include "ydpd.h"
#include "ydptypes.h"
#include "dictinstance.h"
#include <Plasma/ToolTipContent>
#include <Plasma/ToolTipManager>

PlasmaYdpd::PlasmaYdpd(QObject *parent, const QVariantList &args)
        :Plasma::PopupApplet(parent, args), m_pMainWidget(NULL)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPopupIcon("accessories-dictionary");
}

PlasmaYdpd::~PlasmaYdpd()
{
    delete widget();
}

QWidget* PlasmaYdpd::widget()
{
    if (NULL == m_pMainWidget)
    {
        m_pMainWidget = new Ydpd();
        connect(m_pMainWidget, SIGNAL(alwaysOnTopStateChanged(bool)), this, SLOT(onAlwaysOnTopStateChanged(bool)));
        connect(m_pMainWidget, SIGNAL(clipboardChanged()), this, SLOT(showPopup()));
        connect(m_pMainWidget, SIGNAL(dictionaryChanged(int)), this, SLOT(onDictionaryChanged(int)));
        m_pMainWidget->init();
        m_pMainWidget->setAttribute(Qt::WA_NoSystemBackground);
    }
   
    return m_pMainWidget;
}

void PlasmaYdpd::onAlwaysOnTopStateChanged(bool checked)
{
    setPassivePopup(checked);
}

void PlasmaYdpd::onDictionaryChanged(int type)
{
    Plasma::ToolTipContent toolTipData;
    DictInst instance = DictInstance::dictInst(YdpTypes::typeToVersion(type));
    toolTipData.setMainText("Ydpd");
    toolTipData.setSubText(instance.description());
    toolTipData.setImage(instance.iconName(YdpTypes::typeToDirection(type)));
    Plasma::ToolTipManager::self()->setContent(this, toolTipData);
}

#include "build/plasma-ydpd.moc"
