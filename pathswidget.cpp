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

#include "pathswidget.h"
#include <QFileDialog>

PathsWidget::PathsWidget(QWidget *parent_) :
    QWidget(parent_)
{
    _pathsForm.setupUi(this);
}

void PathsWidget::on_dictionaryComboBox_currentIndexChanged(int index)
{
    int version = _pathsForm.dictionaryComboBox->itemData(index).toInt();
    _pathsForm.dictDirLineEdit->setText(_dataFilesPath[version]);
    _pathsForm.dictDirLineEdit->home(false);
    _pathsForm.sampleDirLineEdit->setText(_voiceSamplesPath[version]);
    _pathsForm.sampleDirLineEdit->home(false);
}

void PathsWidget::addDictionary(QString iconName, QString description, int version)
{
    _pathsForm.dictionaryComboBox->addItem(QIcon(iconName), description, QVariant(version));
}

void PathsWidget::setUserSelectedPath(VersionToStringMap& stringMap, QLineEdit* lineEdit)
{
    QFileDialog fileDialog(0, "", QDir::currentPath());
    fileDialog.setFileMode(QFileDialog::Directory);
    if (fileDialog.exec() == QDialog::Accepted)
    {
        QString path = QDir::toNativeSeparators(fileDialog.directory().path());
        int index = _pathsForm.dictionaryComboBox->currentIndex();
        lineEdit->setText(path);
        int version = _pathsForm.dictionaryComboBox->itemData(index).toInt();
        stringMap[version] = path;
    }
}

void PathsWidget::on_dictDirToolButton_clicked()
{
    setUserSelectedPath(_dataFilesPath, _pathsForm.dictDirLineEdit);
}

void PathsWidget::on_sampleDirToolButton_clicked()
{
    setUserSelectedPath(_voiceSamplesPath, _pathsForm.sampleDirLineEdit);
}
