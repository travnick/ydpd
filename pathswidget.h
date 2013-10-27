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

#ifndef PATHSWIDGET_H
#define PATHSWIDGET_H

#include <QMap>
#include <QWidget>
#include "ui_pathsForm.h"

typedef QMap<int, QString> VersionToStringMap;

class PathsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PathsWidget(QWidget *parent = 0);
    QString dictionaryPath(int version) { return _dataFilesPath[version]; }
    void setDictionaryPath(int version, QString path) { _dataFilesPath[version] = path; }
    QString samplesPath(int version) { return _voiceSamplesPath[version]; }
    void setSamplesPath(int version, QString path) { _voiceSamplesPath[version] = path; }
    void addDictionary(QString iconName, QString description, int version);
private:
    Ui::pathsForm _pathsForm;
    VersionToStringMap _dataFilesPath;
    VersionToStringMap _voiceSamplesPath;
    void setUserSelectedPath(VersionToStringMap& stringMap, QLineEdit* lineEdit);
public slots:
    void on_dictionaryComboBox_currentIndexChanged(int index);
private slots:
    void on_dictDirToolButton_clicked();
    void on_sampleDirToolButton_clicked();
};

#endif // PATHSWIDGET_H
