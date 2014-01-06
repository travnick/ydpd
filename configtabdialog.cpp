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

#include "configtabdialog.h"
#include "dictinstance.h"
#include <QDialogButtonBox>
#include "ydptypes.h"
#include "config.h"

ConfigTabDialog::ConfigTabDialog(QWidget *parent_) :
    QDialog(parent_)
{
    QWidget *pStartupWidget = new QWidget;
    _startupForm.setupUi(pStartupWidget);

    QWidget *pHistoryWidget = new QWidget;
    _historyForm.setupUi(pHistoryWidget);

    QTabWidget *pTabWidget = new QTabWidget;
    pTabWidget->addTab(pStartupWidget, QIcon(":/res/fork.png"), "Start");
    pTabWidget->addTab(&_pathsWidget, QIcon(":/res/drive-harddisk.png"), QString::fromUtf8("Ścieżki"));
    pTabWidget->addTab(pHistoryWidget, QIcon(":/res/document-save.png"), "Historia");
    pTabWidget->setUsesScrollButtons(false);

    QDialogButtonBox *pButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(pButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(pTabWidget);
    pMainLayout->addWidget(pButtonBox);
    setLayout(pMainLayout);
    setWindowTitle("Konfiguracja");
    setWindowIcon(QIcon(":/res/configure.png"));

    foreach (int version, DictInstance::versionList())
    {
        const DictInst& instance = DictInstance::dictInst(version);
        _pathsWidget.addDictionary(instance.getForeignIconName(),
                                    instance.getDescription(), version);
        _startupForm.selectedComboBox->addItem(QIcon(instance.getForeignIconName()),
                                                instance.getDescription(),
                                                QVariant(version+YdpTypes::Foreign));
        _startupForm.selectedComboBox->addItem(QIcon(instance.getNativeIconName()),
                                                instance.getDescription(),
                                                QVariant(version+YdpTypes::Native));
    }
}

void ConfigTabDialog::setConfig(const Config& config)
{
    foreach (int version, DictInstance::versionList())
    {
        _pathsWidget.setDictionaryPath(version, config.getDictionaryPath(version));
        _pathsWidget.setSamplesPath(version, config.samplesPath(version));
    }
    (config.dictionaryToOpen() ? _startupForm.selectedRadioButton : _startupForm.recentlyUsedRadioButton)->setChecked(true);
    int index = _startupForm.selectedComboBox->findData(config.selectedDictionary());
    _startupForm.selectedComboBox->setCurrentIndex(index);
    _historyForm.historyCheckBox->setChecked(config.storeHistory());
    _historyForm.noOfItemsSpinBox->setValue(config.maxNumberOfHistoryItems());
    _pathsWidget.on_dictionaryComboBox_currentIndexChanged(0);
    _config = config;
}

Config ConfigTabDialog::getConfig()
{
    _config.setDictionaryToOpen(_startupForm.selectedRadioButton->isChecked());
    int index = _startupForm.selectedComboBox->currentIndex();
    _config.setSelectedDictionary(_startupForm.selectedComboBox->itemData(index).toInt());
    _config.setStoreHistory(_historyForm.historyCheckBox->isChecked());
    _config.setMaxNumberOfHistoryItems(_historyForm.noOfItemsSpinBox->value());
    foreach (int version, DictInstance::versionList())
    {
        _config.setDictionaryPath(version, _pathsWidget.dictionaryPath(version));
        _config.setSamplesPath(version, _pathsWidget.samplesPath(version));
    }
    return _config;
}
