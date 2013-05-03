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

#ifndef Ydpd_HEADER
#define Ydpd_HEADER

#include <Plasma/PopupApplet>

class Ydpd;

class PlasmaYdpd : public Plasma::PopupApplet
{
    Q_OBJECT
public:
    PlasmaYdpd(QObject *parent, const QVariantList &args);
    ~PlasmaYdpd();
    QWidget *widget();
private:
    Ydpd *m_pMainWidget;
private slots:
    void onAlwaysOnTopStateChanged(bool checked);
    void onDictionaryChanged(int type);
};

K_EXPORT_PLASMA_APPLET(ydpd, PlasmaYdpd)
#endif
