/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "programmetimerview.h"
#include "instantiator.h"
#include "programmetimer.h"
#include "programmetimerdialog.h"
#include "programmetimermodel.h"
#include "programmetimers.h"
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

ProgrammeTimerView::ProgrammeTimerView(const QString &remoteId, QWidget *parent) :
    QWidget(parent),
    m_timerList(Instantiator::createProgrammeTimerList(remoteId, this)),
    m_timers(Instantiator::createProgrammeTimers(remoteId, this)),
    m_model(new ProgrammeTimerModel(this)),
    m_view(new QTreeView(this)),
    m_toolBar(new QToolBar(this)),
    m_contextMenu(new QMenu(this)),
    m_newAction(new QAction(QIcon::fromTheme("list-add"), tr("&New timer"), this)),
    m_cleanupAction(new QAction(QIcon::fromTheme("trash-empty"), tr("&Cleanup timers"), this)),
    m_clearAction(new QAction(QIcon::fromTheme("trash-empty"), tr("C&lear timers"), this)),
    m_reloadAction(new QAction(QIcon::fromTheme("view-refresh"), tr("Reload"), this)),
    m_editAction(new QAction(QIcon::fromTheme("document-properties"), tr("&Edit timer"), this)),
    m_removeAction(new QAction(QIcon::fromTheme("trash-empty"), tr("&Remove timer"), this)),
    m_layout(new QVBoxLayout(this)),
    m_remoteId(remoteId)
{
    m_model->setTimers(m_timerList);
    
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QTreeView::SelectRows);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    m_view->setEditTriggers(QTreeView::NoEditTriggers);
    m_view->setItemsExpandable(false);
    m_view->setIndentation(0);
    m_view->setUniformRowHeights(true);
    m_view->setAllColumnsShowFocus(true);

    m_toolBar->addAction(m_newAction);
    m_toolBar->addAction(m_cleanupAction);
    m_toolBar->addAction(m_clearAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_reloadAction);

    m_contextMenu->addAction(m_editAction);
    m_contextMenu->addAction(m_removeAction);

    m_reloadAction->setShortcuts(QKeySequence::Refresh);
    
    m_layout->addWidget(m_view);
    m_layout->addWidget(m_toolBar);

    connect(m_timerList, SIGNAL(finished()), this, SLOT(onTimerListFinished()));
    connect(m_timers, SIGNAL(finished()), this, SLOT(onTimersFinished()));
    connect(m_view, SIGNAL(activated(QModelIndex)), this, SLOT(editTimer(QModelIndex)));
    connect(m_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(m_newAction, SIGNAL(triggered()), this, SLOT(addTimer()));
    connect(m_cleanupAction, SIGNAL(triggered()), this, SLOT(cleanupTimers()));
    connect(m_clearAction, SIGNAL(triggered()), this, SLOT(clearTimers()));
    connect(m_reloadAction, SIGNAL(triggered()), m_model, SLOT(reload()));
    connect(m_editAction, SIGNAL(triggered()), this, SLOT(editTimer()));
    connect(m_removeAction, SIGNAL(triggered()), this, SLOT(removeTimer()));
}

void ProgrammeTimerView::reload() {
    m_model->reload();
}

void ProgrammeTimerView::addTimer() {
    ProgrammeTimerDialog dialog(m_remoteId, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        ProgrammeTimer timer;
        timer.setDescription(dialog.description());
        timer.setStartTime(dialog.startTime());
        timer.setEndTime(dialog.endTime());
        timer.setChannelId(dialog.channelId());
        timer.setChannelTitle(dialog.channelTitle());
        timer.setEventType(dialog.eventType());
        timer.setRecurringDays(dialog.recurringDays());
        timer.setAction(dialog.action());
        timer.setAfterAction(dialog.afterAction());
        m_timers->addProgrammeTimer(&timer);
    };
}

void ProgrammeTimerView::cleanupTimers() {
    m_timers->cleanupProgrammeTimers();
}

void ProgrammeTimerView::clearTimers() {
    m_timers->clearProgrammeTimers();
}

void ProgrammeTimerView::editTimer() {
    if (m_view->currentIndex().isValid()) {
        editTimer(m_view->currentIndex());
    }
}

void ProgrammeTimerView::editTimer(const QModelIndex &index) {
    ProgrammeTimerDialog dialog(m_remoteId, this);
    dialog.setDescription(index.data(ProgrammeTimerModel::DescriptionRole).toString());
    dialog.setStartTime(index.data(ProgrammeTimerModel::StartTimeRole).toDateTime());
    dialog.setEndTime(index.data(ProgrammeTimerModel::EndTimeRole).toDateTime());
    dialog.setChannelId(index.data(ProgrammeTimerModel::ChannelIdRole).toString());
    dialog.setChannelTitle(index.data(ProgrammeTimerModel::ChannelTitleRole).toString());
    dialog.setEventType(ProgrammeTimer::EventType(index.data(ProgrammeTimerModel::EventTypeRole).toInt()));
    dialog.setRecurringDays(ProgrammeTimer::Days(index.data(ProgrammeTimerModel::RecurringDaysRole).toInt()));
    dialog.setAction(ProgrammeTimer::Action(index.data(ProgrammeTimerModel::ActionRole).toInt()));
    dialog.setAfterAction(ProgrammeTimer::Action(index.data(ProgrammeTimerModel::AfterActionRole).toInt()));
    
    if (dialog.exec() == QDialog::Accepted) {
        ProgrammeTimer oldTimer;
        oldTimer.setDescription(index.data(ProgrammeTimerModel::DescriptionRole).toString());
        oldTimer.setStartTime(index.data(ProgrammeTimerModel::StartTimeRole).toDateTime());
        oldTimer.setEndTime(index.data(ProgrammeTimerModel::EndTimeRole).toDateTime());
        oldTimer.setChannelId(index.data(ProgrammeTimerModel::ChannelIdRole).toString());
        oldTimer.setChannelTitle(index.data(ProgrammeTimerModel::ChannelTitleRole).toString());
        oldTimer.setEventType(ProgrammeTimer::EventType(index.data(ProgrammeTimerModel::EventTypeRole).toInt()));
        oldTimer.setRecurringDays(ProgrammeTimer::Days(index.data(ProgrammeTimerModel::RecurringDaysRole).toInt()));
        oldTimer.setAction(ProgrammeTimer::Action(index.data(ProgrammeTimerModel::ActionRole).toInt()));
        oldTimer.setAfterAction(ProgrammeTimer::Action(index.data(ProgrammeTimerModel::AfterActionRole).toInt()));
        
        ProgrammeTimer newTimer;
        newTimer.setDescription(dialog.description());
        newTimer.setStartTime(dialog.startTime());
        newTimer.setEndTime(dialog.endTime());
        newTimer.setChannelId(dialog.channelId());
        newTimer.setChannelTitle(dialog.channelTitle());
        newTimer.setEventType(dialog.eventType());
        newTimer.setRecurringDays(dialog.recurringDays());
        newTimer.setAction(dialog.action());
        newTimer.setAfterAction(dialog.afterAction());
        m_timers->updateProgrammeTimer(&oldTimer, &newTimer);
    }
}

void ProgrammeTimerView::removeTimer() {
    if (m_view->currentIndex().isValid()) {
        removeTimer(m_view->currentIndex());
    }
}

void ProgrammeTimerView::removeTimer(const QModelIndex &index) {
    ProgrammeTimer timer;
    timer.setId(index.data(ProgrammeTimerModel::IdRole).toString());
    timer.setDescription(index.data(ProgrammeTimerModel::DescriptionRole).toString());
    timer.setStartTime(index.data(ProgrammeTimerModel::StartTimeRole).toDateTime());
    timer.setEndTime(index.data(ProgrammeTimerModel::EndTimeRole).toDateTime());
    timer.setChannelId(index.data(ProgrammeTimerModel::ChannelIdRole).toString());
    timer.setChannelTitle(index.data(ProgrammeTimerModel::ChannelTitleRole).toString());
    timer.setEventType(ProgrammeTimer::EventType(index.data(ProgrammeTimerModel::EventTypeRole).toInt()));
    timer.setRecurringDays(ProgrammeTimer::Days(index.data(ProgrammeTimerModel::RecurringDaysRole).toInt()));
    timer.setAction(ProgrammeTimer::Action(index.data(ProgrammeTimerModel::ActionRole).toInt()));
    timer.setAfterAction(ProgrammeTimer::Action(index.data(ProgrammeTimerModel::AfterActionRole).toInt()));
    m_timers->removeProgrammeTimer(&timer);
}

void ProgrammeTimerView::showContextMenu(const QPoint &pos) {
    if (m_view->currentIndex().isValid()) {
        m_contextMenu->popup(m_view->mapToGlobal(pos), m_editAction);
    }
}

void ProgrammeTimerView::onTimerListFinished() {
    if (m_timerList->status() == Request::Error) {
        QMessageBox::critical(this, tr("Error"), m_timerList->errorString());
    }
}

void ProgrammeTimerView::onTimersFinished() {
    switch (m_timers->status()) {
    case Request::Ready:
        m_model->reload();
        break;
    case Request::Error:
        QMessageBox::critical(this, tr("Error"), m_timers->errorString());
        break;
    default:
        break;
    }
}
