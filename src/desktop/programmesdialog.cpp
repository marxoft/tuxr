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

#include "programmesdialog.h"
#include "instantiator.h"
#include "programmedialog.h"
#include "programmemodel.h"
#include "programmetimerdialog.h"
#include "programmetimers.h"
#include <QAction>
#include <QDialogButtonBox>
#include <QMenu>
#include <QMessageBox>
#include <QTreeView>
#include <QVBoxLayout>

ProgrammesDialog::ProgrammesDialog(const QString &remoteId, QWidget *parent) :
    QDialog(parent),
    m_programmes(Instantiator::createProgrammeList(remoteId, this)),
    m_timers(Instantiator::createProgrammeTimers(remoteId, this)),
    m_model(new ProgrammeModel(this)),
    m_view(new QTreeView(this)),
    m_contextMenu(new QMenu(this)),
    m_showAction(new QAction(QIcon::fromTheme("document-properties"), tr("Show &details"), this)),
    m_timerAction(new QAction(QIcon::fromTheme("task-new"), tr("Set &timer"), this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, this)),
    m_layout(new QVBoxLayout(this)),
    m_remoteId(remoteId)
{
    setWindowTitle(tr("EPG"));
    setMinimumSize(QSize(600, 400));

    m_model->setProgrammes(m_programmes);
    
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QTreeView::SelectRows);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    m_view->setEditTriggers(QTreeView::NoEditTriggers);
    m_view->setItemsExpandable(false);
    m_view->setIndentation(0);
    m_view->setUniformRowHeights(true);
    m_view->setAllColumnsShowFocus(true);

    m_contextMenu->addAction(m_showAction);
    m_contextMenu->addAction(m_timerAction);

    m_layout->addWidget(m_view);
    m_layout->addWidget(m_buttonBox);

    connect(m_programmes, SIGNAL(finished()), this, SLOT(onProgrammesFinished()));
    connect(m_timers, SIGNAL(finished()), this, SLOT(onTimersFinished()));
    connect(m_view, SIGNAL(activated(QModelIndex)), this, SLOT(showProgramme(QModelIndex)));
    connect(m_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(m_showAction, SIGNAL(triggered()), this, SLOT(showProgramme()));
    connect(m_timerAction, SIGNAL(triggered()), this, SLOT(setTimer()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ProgrammesDialog::reload(const QString &ref) {
    m_model->reload(ref);
}

void ProgrammesDialog::showProgramme() {
    if (m_view->currentIndex().isValid()) {
        showProgramme(m_view->currentIndex());
    }
}

void ProgrammesDialog::showProgramme(const QModelIndex &index) {
    ProgrammeDialog dialog(this);
    dialog.setTitle(index.data(ProgrammeModel::TitleRole).toString());
    dialog.setDescription(index.data(ProgrammeModel::DescriptionRole).toString());
    dialog.setStartTime(index.data(ProgrammeModel::StartTimeRole).toDateTime());
    dialog.setEndTime(index.data(ProgrammeModel::EndTimeRole).toDateTime());
    dialog.setChannelTitle(index.data(ProgrammeModel::ChannelTitleRole).toString());
    dialog.exec();
}

void ProgrammesDialog::setTimer() {
    if (m_view->currentIndex().isValid()) {
        setTimer(m_view->currentIndex());
    }
}

void ProgrammesDialog::setTimer(const QModelIndex &index) {
    ProgrammeTimerDialog dialog(m_remoteId, this);
    dialog.setStartTime(index.data(ProgrammeModel::StartTimeRole).toDateTime());
    dialog.setEndTime(index.data(ProgrammeModel::EndTimeRole).toDateTime());
    dialog.setChannelId(index.data(ProgrammeModel::ChannelIdRole).toString());
    dialog.setChannelTitle(index.data(ProgrammeModel::ChannelTitleRole).toString());

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
    }
}

void ProgrammesDialog::showContextMenu(const QPoint &pos) {
    if (m_view->currentIndex().isValid()) {
        m_contextMenu->popup(m_view->mapToGlobal(pos), m_showAction);
    }
}

void ProgrammesDialog::onProgrammesFinished() {
    if (m_programmes->status() == Request::Error) {
        QMessageBox::critical(this, tr("Error"), m_programmes->errorString());
    }
}

void ProgrammesDialog::onTimersFinished() {
    if (m_timers->status() == Request::Error) {
        QMessageBox::critical(this, tr("Error"), m_timers->errorString());
    }
}
