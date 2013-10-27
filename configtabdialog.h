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

#ifndef CONFIGTABDIALOG_H
#define CONFIGTABDIALOG_H

#include <QDialog>
#include "ui_startupForm.h"
#include "ui_historyForm.h"
#include "pathswidget.h"
#include "config.h"

class ConfigTabDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigTabDialog(QWidget *parent = 0);
    void setConfig(const Config& config);
    Config config();
private:
    Ui_startupForm _startupForm;
    Ui_historyForm _historyForm;
    PathsWidget _pathsWidget;
    Config _config;
};

#endif // CONFIGTABDIALOG_H
