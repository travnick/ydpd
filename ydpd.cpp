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

#include "ydpd.h"
#include "ui_ydpdForm.h"
#include "configtabdialog.h"
#include "configmanager.h"
#include "dictmanager.h"
#ifdef Q_WS_X11
#include <phonon/MediaObject>
#else
#include <QSound>
#endif
#include <QMessageBox>
#include <QLineEdit>
#include <QShortcut>
#include <QAction>
#include <QFile>
#include <QMenu>

const int Ydpd::_addItemToHistoryTimerInterval  = 2000;

Ydpd::Ydpd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ydpd)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);

    connect(&IDictManager::instance(), SIGNAL(dictionaryChanged(int)), this, SLOT(onDictionaryChanged(int)));
    connect(ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(onSplitterChanged()));
}

void Ydpd::onSplitterChanged()
{
    IConfigManager::instance().config().setSplitters(QPair<int, int>(ui->splitter->sizes().at(0),
                                                                     ui->splitter->sizes().at(1)));
}

void Ydpd::init()
{
    IConfigManager& confMan = IConfigManager::instance();
#ifndef PLASMA
    setGeometry(confMan.config().geometry());
#endif
    QList<int> list;
    list.append(confMan.config().getSplitters().first);
    list.append(confMan.config().getSplitters().second);
    ui->splitter->setSizes(list);
    ui->historyComboBox->setCompleter(nullptr);

    IDictManager& dictMan = IDictManager::instance();
    ui->entriesListView->setModel(dictMan.entriesModel());
    ui->dictionaryComboBox->setModel(dictMan.dictionaryModel());
    ui->historyComboBox->setModel(dictMan.historyModel());

    createMenu();
    initWidgets();

    _clipboardAction->setChecked(confMan.config().clipboard());
    _selfSelectionAction->setChecked(confMan.config().selfSelection());
    _alwaysOnTopAction->setChecked(confMan.config().alwaysOnTop());
    _autoShowWindowAction->setChecked(confMan.config().autoShowWindow());

    QShortcut *changeDictShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this);
    connect(changeDictShortcut, SIGNAL(activated()), this, SLOT(changeDictionary()));
    QShortcut *clearLineEditShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(clearLineEditShortcut, SIGNAL(activated()), this, SLOT(clearLineEdit()));

    _addItemToHistoryTimer.setSingleShot(true);
    _addItemToHistoryTimer.setInterval(_addItemToHistoryTimerInterval);

    connect(&_addItemToHistoryTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(ui->historyComboBox->lineEdit(), SIGNAL(returnPressed()), this, SLOT(lineEdit_returnPressed()));
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    connect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(ui->optionsToolButton, SIGNAL(clicked()), this, SLOT(configuration()));

    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void Ydpd::createMenu()
{
    QMenu *menu = new QMenu(this);

    _clipboardAction = new QAction(QIcon(":/res/fifteenpuzzle.svgz"), QString::fromUtf8("Śledź schowek"), this);
    connect(_clipboardAction, SIGNAL(toggled(bool)), this, SLOT(updateCheckboxes()));
    connect(_clipboardAction, SIGNAL(toggled(bool)), &IConfigManager::instance(), SLOT(onClipboardChanged(bool)));
    _clipboardAction->setCheckable(true);
    menu->addAction(_clipboardAction);

    _selfSelectionAction = new QAction(QIcon(":/res/preferences-desktop-mouse.png"), QString::fromUtf8("Śledź własne zaznaczenia"), this);
    connect(_selfSelectionAction, SIGNAL(toggled(bool)), &IConfigManager::instance(), SLOT(onSelfSelectionChanged(bool)));
    _selfSelectionAction->setCheckable(true);
    menu->addAction(_selfSelectionAction);
    _selfSelectionAction->setVisible(QApplication::clipboard()->supportsSelection());

    _alwaysOnTopAction = new QAction(QIcon(":/res/document-encrypt.png"), QString::fromUtf8("Zawsze na wierzchu"), this);
    connect(_alwaysOnTopAction, SIGNAL(toggled(bool)), this, SLOT(alwaysOnTop_stateChanged(bool)));
    connect(_alwaysOnTopAction, SIGNAL(toggled(bool)), &IConfigManager::instance(), SLOT(onAlwaysOnTopChanged(bool)));
    _alwaysOnTopAction->setCheckable(true);
    menu->addAction(_alwaysOnTopAction);
    
    _autoShowWindowAction = new QAction(QIcon(":/res/apport.svg"), QString::fromUtf8("Automatycznie pokaż okno"), this);
    connect(_autoShowWindowAction, SIGNAL(toggled(bool)), &IConfigManager::instance(), SLOT(onAutoShowWindowChanged(bool)));
    _autoShowWindowAction->setCheckable(true);
    menu->addAction(_autoShowWindowAction);
#ifndef PLASMA
    _autoShowWindowAction->setVisible(false);
#endif

    menu->addSeparator();

    QAction *aboutAction = new QAction(QIcon(":/res/accessories-dictionary.png"), "O aplikacji", this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    menu->addAction(aboutAction);

    ui->optionsToolButton->setMenu(menu);
}

void Ydpd::connectSignalsToSlots()
{
    connect(ui->historyComboBox->lineEdit(), SIGNAL(textEdited(const QString&)), this, SLOT(lineEdit_textEdited(const QString&)));
    connect(ui->historyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(historyComboBox_currentIndexChanged(int)));
    connect(ui->dictionaryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dictionaryComboBox_currentIndexChanged(int)));
    connect(ui->nativeRadioButton, SIGNAL(toggled(bool)), this, SLOT(nativeRadioButton_toggled(bool)));
    connect(ui->foreignRadioButton, SIGNAL(toggled(bool)), this, SLOT(foreignRadioButton_toggled(bool)));
    connect(ui->entriesListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(entriesListView_clicked(const QModelIndex&)));
}

void Ydpd::disconnectSignalsFromSlots()
{
    disconnect(ui->historyComboBox->lineEdit(), SIGNAL(textEdited(const QString&)), this, SLOT(lineEdit_textEdited(const QString&)));
    disconnect(ui->historyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(historyComboBox_currentIndexChanged(int)));
    disconnect(ui->dictionaryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dictionaryComboBox_currentIndexChanged(int)));
    disconnect(ui->nativeRadioButton, SIGNAL(toggled(bool)), this, SLOT(nativeRadioButton_toggled(bool)));
    disconnect(ui->foreignRadioButton, SIGNAL(toggled(bool)), this, SLOT(foreignRadioButton_toggled(bool)));
    disconnect(ui->entriesListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(entriesListView_clicked(const QModelIndex&)));
}

void Ydpd::initWidgets()
{
    bool widgetsEnabled = IDictManager::instance().isCurrentDictValid();

    disconnectSignalsFromSlots();
    clearWidgets();
    if (widgetsEnabled)
    {
        historyComboBoxInit();
        dictionaryComboBoxInit();
        entriesListViewInit();
        descriptionTextEditInit();
        radioButtonsInit();
    }
    enableWidgets(widgetsEnabled);
    connectSignalsToSlots();
}

Ydpd::~Ydpd()
{
    disconnectSignalsFromSlots();
    delete ui;
}

void Ydpd::enableWidgets(bool enable)
{
    ui->dictionaryComboBox->setEnabled(enable && ui->dictionaryComboBox->count() > 1);
    ui->historyComboBox->setEnabled(enable);
    ui->entriesListView->setEnabled(enable);
    ui->descriptionTextEdit->setEnabled(enable);
    ui->foreignRadioButton->setEnabled(enable);
    ui->nativeRadioButton->setEnabled(enable);
    ui->prevToolButton->setEnabled(enable && ui->historyComboBox->currentIndex() && ui->historyComboBox->count() &&
                                             ui->historyComboBox->currentIndex() != -1);
    ui->nextToolButton->setEnabled(enable && ui->historyComboBox->currentIndex() != ui->historyComboBox->count()-1);
    enablePlayButton(ui->entriesListView->currentIndex().row());
    _clipboardAction->setEnabled(enable);
    _selfSelectionAction->setEnabled(enable);
    _alwaysOnTopAction->setEnabled(enable);
    _autoShowWindowAction->setEnabled(enable);
    updateCheckboxes(enable);
}

void Ydpd::clearWidgets()
{
    IDictManager& man = IDictManager::instance();
    man.clearDictionary();
    man.clearHistory();
    ui->historyComboBox->clearEditText();
    man.clearEntries();
    ui->descriptionTextEdit->clear();
    ui->foreignRadioButton->setChecked(false);
    ui->foreignRadioButton->setIcon(QIcon());
    ui->nativeRadioButton->setChecked(false);
    ui->prevToolButton->setChecked(false);
    ui->nextToolButton->setChecked(false);
    ui->playToolButton->setChecked(false);
    _alwaysOnTopAction->setChecked(false);
    _clipboardAction->setChecked(false);
    _selfSelectionAction->setChecked(false);
    _autoShowWindowAction->setChecked(false);
}

void Ydpd::historyComboBoxInit()
{
    IDictManager::instance().initHistory();
    ui->historyComboBox->setCurrentIndex(-1);
}

void Ydpd::dictionaryComboBoxInit()
{
    IDictManager::instance().initDictionary();
    dictionaryComboBoxSetVersion();
}

void Ydpd::dictionaryComboBoxSetVersion()
{
    ui->dictionaryComboBox->setCurrentIndex(IDictManager::instance().currentDictionaryIndex());
}

void Ydpd::entriesListViewInit()
{
    IDictManager& man = IDictManager::instance();
    man.initEntries();
    entriesListViewSetIndex(man.entryToIndex(ui->historyComboBox->currentText()));
}

void Ydpd::entriesListViewSetIndex(int index)
{
    ui->entriesListView->setCurrentIndex(ui->entriesListView->model()->index(index, 0));
    enablePlayButton(index);
}

void Ydpd::descriptionTextEditInit()
{
    descriptionTextEditSetContents(ui->entriesListView->currentIndex().row());
}

void Ydpd::descriptionTextEditSetContents(int index)
{
    ui->descriptionTextEdit->setText(IDictManager::instance().description(index));
}

void Ydpd::radioButtonsInit()
{
    foreignRadioButtonSetIconAndTooltip();
    (IDictManager::instance().isDirectionForeign() ? ui->foreignRadioButton : ui->nativeRadioButton)->setChecked(true);
}

void Ydpd::foreignRadioButtonSetIconAndTooltip()
{
    IDictManager& man = IDictManager::instance();
    ui->foreignRadioButton->setIcon(QIcon(man.foreignIconName()));
    ui->foreignRadioButton->setToolTip(man.tooltip());
}

void Ydpd::entriesListView_clicked(const QModelIndex& index)
{
    _addItemToHistoryTimer.stop();
    disconnectSignalsFromSlots();
    const int entryIndex = index.row();
    ui->historyComboBox->setEditText(IDictManager::instance().entriesText(index.row()));
    descriptionTextEditSetContents(entryIndex);
    connectSignalsToSlots();
    addCurrentEntryToHistory();
    setFocusOnLineEdit();
    enablePlayButton(index.row());
}

void Ydpd::on_entriesListView_doubleClicked(const QModelIndex& index)
{
    if (enablePlayButton(index.row()))
        on_playToolButton_clicked();
}

void Ydpd::dictionaryComboBox_currentIndexChanged(int index)
{
    if (index != -1)
    {
        IDictManager& man = IDictManager::instance();
        man.setVersion(index);
        const QString currentText = ui->historyComboBox->currentText();
        const int entryIndex = man.entryToIndex(currentText);
        entriesListViewSetIndex(entryIndex);
        descriptionTextEditSetContents(entryIndex);
        foreignRadioButtonSetIconAndTooltip();
        setFocusOnLineEdit();
    }
}

void Ydpd::lineEdit_textEdited(const QString& text)
{
    _addItemToHistoryTimer.start();
    const int entryIndex = IDictManager::instance().entryToIndex(text);
    const int index = ui->entriesListView->currentIndex().row();
    if (index != entryIndex)
    {
        entriesListViewSetIndex(entryIndex);
        descriptionTextEditSetContents(entryIndex);
    }
}

void Ydpd::historyComboBox_currentIndexChanged(int index)
{
    disconnectSignalsFromSlots();
    IDictManager& man = IDictManager::instance();
    enablePrevNextButtons(index);
    if (man.setType(index))
        dictionaryComboBoxSetVersion();
    radioButtonsInit();
    const int entryIndex = man.entryToIndex(ui->historyComboBox->currentText());
    entriesListViewSetIndex(entryIndex);
    descriptionTextEditSetContents(entryIndex);
    connectSignalsToSlots();
    setFocusOnLineEdit();
}

void Ydpd::on_prevToolButton_clicked()
{
    ui->historyComboBox->setCurrentIndex(ui->historyComboBox->currentIndex()-1);
}

void Ydpd::on_nextToolButton_clicked()
{
    ui->historyComboBox->setCurrentIndex(ui->historyComboBox->currentIndex()+1);
}

void Ydpd::nativeRadioButton_toggled(bool checked)
{
    radioButtonToggled(checked);
}

void Ydpd::foreignRadioButton_toggled(bool checked)
{
    radioButtonToggled(checked);
}

void Ydpd::radioButtonToggled(bool checked)
{
    if (checked)
    {
        IDictManager& man = IDictManager::instance();
        man.invertDirection();
        const QString currentText = ui->historyComboBox->currentText();
        const int entryIndex = man.entryToIndex(currentText);
        entriesListViewSetIndex(entryIndex);
        descriptionTextEditSetContents(entryIndex);
        setFocusOnLineEdit();
    }
}

void Ydpd::lineEdit_returnPressed()
{
    if (_addItemToHistoryTimer.isActive())
    {
        _addItemToHistoryTimer.stop();
        addCurrentEntryToHistory();
    }
}

void Ydpd::timeout()
{
    if (!ui->historyComboBox->currentText().isEmpty())
    {
        addCurrentEntryToHistory();
        ui->historyComboBox->lineEdit()->selectAll();
    }
}

void Ydpd::addCurrentEntryToHistory()
{
    const QString currentText = ui->historyComboBox->currentText();
    IDictManager& man = IDictManager::instance();

    if (ui->historyComboBox->count() && ui->historyComboBox->itemText(0) == currentText &&
        man.isTypeEqual(ui->historyComboBox->itemData(0).toInt()))
        return;

    const int index = 0;
    disconnectSignalsFromSlots();
    man.insertHistoryItem(currentText);
    ui->historyComboBox->setCurrentIndex(index);
    enablePrevNextButtons(index);
    connectSignalsToSlots();
}

void Ydpd::enablePrevNextButtons(int historyEntryIndex)
{
    ui->prevToolButton->setEnabled(historyEntryIndex);
    ui->nextToolButton->setEnabled(historyEntryIndex != ui->historyComboBox->count()-1);
}

void Ydpd::dataChanged()
{
    if (_clipboardAction->isChecked())
    {
        _addItemToHistoryTimer.stop();
        handleClipboardChange(QClipboard::Clipboard);
    }
}

void Ydpd::selectionChanged()
{
    if (_clipboardAction->isChecked() ||
       (_selfSelectionAction->isChecked() && ui->descriptionTextEdit->textCursor().hasSelection()))
    {
        _addItemToHistoryTimer.stop();
        handleClipboardChange(QClipboard::Selection);
    }
}

void Ydpd::handleClipboardChange(QClipboard::Mode mode)
{
    const QString text = QApplication::clipboard()->text(mode);
    ui->historyComboBox->lineEdit()->setText(text);
    addCurrentEntryToHistory();
    const int entryIndex = IDictManager::instance().entryToIndex(text);
    entriesListViewSetIndex(entryIndex);
    descriptionTextEditSetContents(entryIndex);
    setFocusOnLineEdit();
    if (_clipboardAction->isChecked() && _autoShowWindowAction->isChecked() && !_alwaysOnTopAction->isChecked())
        emit clipboardChanged();
}

void Ydpd::alwaysOnTop_stateChanged(bool checked)
{
#ifndef PLASMA
    if (checked)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);

    show();
#endif
    updateCheckboxes();
    emit alwaysOnTopStateChanged(checked);
}

void Ydpd::setFocusOnLineEdit()
{
    ui->historyComboBox->lineEdit()->selectAll();
    ui->historyComboBox->lineEdit()->setFocus();
}

bool Ydpd::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate)
        setFocusOnLineEdit();

    return QWidget::event(event);
}

void Ydpd::moveEvent (QMoveEvent* event)
{
    IConfigManager::instance().config().setGeometry(geometry());
}


void Ydpd::resizeEvent (QResizeEvent* event)
{
    IConfigManager::instance().config().setGeometry(geometry());
}

void Ydpd::configuration()
{
    ConfigTabDialog configDialog;
    IConfigManager&  confMan = IConfigManager::instance();

    configDialog.setConfig(confMan.config());
    if (configDialog.exec() == QDialog::Accepted)
    {
        confMan.setConfig(configDialog.config());
        confMan.save();
        IDictManager::instance().init();
        initWidgets();
    }
}

QString Ydpd::wavFilePath(int index)
{
    IDictManager& man = IDictManager::instance();
    const QString wavFormat = man.wavFormat();
    return man.samplesPath() + QString(wavFormat).arg((index+1)/1000, 3, 10, QChar('0')).arg(index+1, 6, 10, QChar('0'));
}

bool Ydpd::enablePlayButton(int index)
{
    IDictManager& man = IDictManager::instance();
    const bool enable = index != -1 && man.isDirectionForeign() && QFile::exists(wavFilePath(index));
    ui->playToolButton->setEnabled(enable);
    return enable;
}

void Ydpd::on_playToolButton_clicked()
{
    int index = ui->entriesListView->currentIndex().row();
#ifdef Q_WS_X11
    Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(wavFilePath(index)))->play();
#else
    QSound::play(wavFilePath(index));
#endif
}

void Ydpd::changeDictionary()
{
    if (ui->dictionaryComboBox->count() <= 1)
        return;

    const int index = (ui->dictionaryComboBox->currentIndex() + 1) % ui->dictionaryComboBox->count();
    ui->dictionaryComboBox->setCurrentIndex(index);
}

void Ydpd::clearLineEdit()
{
    if (ui->descriptionTextEdit->hasFocus())
        setFocusOnLineEdit();
    else
        ui->historyComboBox->lineEdit()->clear();
}

void Ydpd::onDictionaryChanged(int type)
{
    emit dictionaryChanged(type);
    IConfigManager::instance().config().setRecentDictionary(type);
}

void Ydpd::about()
{
    QMessageBox::about(this, "O aplikacji", "version 0.5 \n\nMarcin Nawrocki\nmar.nawrocki@gmail.com");
}

void Ydpd::updateCheckboxes(bool enable)
{
    _clipboardAction->setEnabled(enable);
    _selfSelectionAction->setEnabled(enable && !_clipboardAction->isChecked());
    _alwaysOnTopAction->setEnabled(enable);
    _autoShowWindowAction->setEnabled(enable && !_alwaysOnTopAction->isChecked() && _clipboardAction->isChecked());
}
