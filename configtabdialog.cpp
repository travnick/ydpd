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

ConfigTabDialog::ConfigTabDialog(QWidget *parent) :
    QDialog(parent)
{
    QWidget *pStartupWidget = new QWidget;
    m_startupForm.setupUi(pStartupWidget);

    QWidget *pHistoryWidget = new QWidget;
    m_historyForm.setupUi(pHistoryWidget);

    QTabWidget *pTabWidget = new QTabWidget;
    pTabWidget->addTab(pStartupWidget, QIcon(":/res/fork.png"), "Start");
    pTabWidget->addTab(&m_pathsWidget, QIcon(":/res/drive-harddisk.png"), QString::fromUtf8("Ścieżki"));
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
        m_pathsWidget.addDictionary(instance.foreignIconName(),
                                    instance.description(), version);
        m_startupForm.selectedComboBox->addItem(QIcon(instance.foreignIconName()),
                                                instance.description(),
                                                QVariant(version+YdpTypes::Foreign));
        m_startupForm.selectedComboBox->addItem(QIcon(instance.nativeIconName()),
                                                instance.description(),
                                                QVariant(version+YdpTypes::Native));
    }
}

void ConfigTabDialog::setConfig(const Config& config)
{
    foreach (int version, DictInstance::versionList())
    {
        m_pathsWidget.setDictionaryPath(version, config.dictionaryPath(version));
        m_pathsWidget.setSamplesPath(version, config.samplesPath(version));
    }
    (config.dictionaryToOpen() ? m_startupForm.selectedRadioButton : m_startupForm.recentlyUsedRadioButton)->setChecked(true);
    int index = m_startupForm.selectedComboBox->findData(config.selectedDictionary());
    m_startupForm.selectedComboBox->setCurrentIndex(index);
    m_historyForm.historyCheckBox->setChecked(config.storeHistory());
    m_historyForm.noOfItemsSpinBox->setValue(config.maxNumberOfHistoryItems());
    m_pathsWidget.on_dictionaryComboBox_currentIndexChanged(0);
    m_config = config;
}

Config ConfigTabDialog::config()
{
    m_config.setDictionaryToOpen(m_startupForm.selectedRadioButton->isChecked());
    int index = m_startupForm.selectedComboBox->currentIndex();
    m_config.setSelectedDictionary(m_startupForm.selectedComboBox->itemData(index).toInt());
    m_config.setStoreHistory(m_historyForm.historyCheckBox->isChecked());
    m_config.setMaxNumberOfHistoryItems(m_historyForm.noOfItemsSpinBox->value());
    foreach (int version, DictInstance::versionList())
    {
        m_config.setDictionaryPath(version, m_pathsWidget.dictionaryPath(version));
        m_config.setSamplesPath(version, m_pathsWidget.samplesPath(version));
    }
    return m_config;
}
