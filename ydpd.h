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

#ifndef YDPD_H
#define YDPD_H

#include <QModelIndex>
#include <QClipboard>
#include <QAction>
#include <QWidget>
#include <QTimer>

namespace Ui {
    class Ydpd;
}

class Ydpd : public QWidget
{
    Q_OBJECT
public:
    explicit Ydpd(QWidget *parent = 0);
    void init();
    ~Ydpd();
signals:
    void alwaysOnTopStateChanged(bool checked);
    void clipboardChanged();
    void dictionaryChanged(int type);
private slots:
    void about();
    void entriesListView_clicked(const QModelIndex& index);
    void on_entriesListView_doubleClicked(const QModelIndex& index);
    void dictionaryComboBox_currentIndexChanged(int index);
    void lineEdit_textEdited(const QString& text);
    void historyComboBox_currentIndexChanged(int index);
    void lineEdit_returnPressed();
    void on_prevToolButton_clicked();
    void on_nextToolButton_clicked();
    void nativeRadioButton_toggled(bool checked);
    void foreignRadioButton_toggled(bool checked);
    void radioButtonToggled(bool checked);
    void alwaysOnTop_stateChanged(bool checked);
    void on_playToolButton_clicked();
    void timeout();
    void dataChanged();
    void selectionChanged();
    void configuration();
    void changeDictionary();
    void clearLineEdit();
    void onSplitterChanged();
    void onDictionaryChanged(int type);
    void updateCheckboxes(bool enable = true);
private:
    QTimer m_addItemToHistoryTimer;
    Ui::Ydpd *ui;
    void initWidgets();
    void createMenu();
    void enableWidgets(bool enable);
    void clearWidgets();
    void connectSignalsToSlots();
    void disconnectSignalsFromSlots();
    void historyComboBoxInit();
    void dictionaryComboBoxInit();
    void entriesListViewInit();
    void descriptionTextEditInit();
    void radioButtonsInit();
    void dictionaryComboBoxSetVersion();
    void entriesListViewSetIndex(int index);
    void descriptionTextEditSetContents(int index);
    void foreignRadioButtonSetIconAndTooltip();
    void addCurrentEntryToHistory();
    void setFocusOnLineEdit();
    void enablePrevNextButtons(int historyEntryIndex);
    void handleClipboardChange(QClipboard::Mode mode);
    bool enablePlayButton(int index);
    bool event(QEvent *);
    void moveEvent (QMoveEvent* event);
    void resizeEvent (QResizeEvent* event);
    QString wavFilePath(int index);
    QAction *m_clipboardAction;
    QAction *m_selfSelectionAction;
    QAction *m_alwaysOnTopAction;
    QAction *m_autoShowWindowAction;
    const static int m_addItemToHistoryTimerInterval;
};

#endif // YDPD_H
