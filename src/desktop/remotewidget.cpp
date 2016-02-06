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

#include "remotewidget.h"
#include "instantiator.h"
#include "programme.h"
#include "programmelist.h"
#include "programmetimerview.h"
#include "programmeview.h"
#include "remoteconfigs.h"
#include "remoteview.h"
#include "screenshotdialog.h"
#include "serviceview.h"
#include "videoplayer.h"
#include <QAction>
#include <QGridLayout>
#include <QStackedWidget>
#include <QTabBar>

RemoteWidget::RemoteWidget(const QString &remoteId, QWidget *parent) :
    QWidget(parent),
    m_programmes(0),
    m_remoteView(0),
    m_serviceView(0),
    m_timerView(0),
    m_player(new VideoPlayer(this)),
    m_programmeView(new ProgrammeView(this)),
    m_tabBar(new QTabBar(this)),
    m_stack(new QStackedWidget(this)),
    m_screenShotAction(new QAction(tr("Take &screenshot"), this)),
    m_layout(new QGridLayout(this)),
    m_remoteId(remoteId)
{
    m_player->setFixedSize(QSize(480, 360));
    m_player->setSource(QUrl::fromUserInput(QString("%1:%2").arg(RemoteConfigs::address(remoteId))
                                                            .arg(RemoteConfigs::streamingPort(remoteId))));

    m_programmeView->hide();
        
    m_tabBar->addTab(tr("Remote"));
    m_tabBar->addTab(tr("Zap"));
    m_tabBar->addTab(tr("Timers"));

    m_screenShotAction->setShortcuts(QKeySequence::Print);
    
    m_layout->addWidget(m_tabBar, 0, 0);
    m_layout->addWidget(m_stack, 1, 0, 2, 1);
    m_layout->addWidget(m_player, 1, 1);
    m_layout->addWidget(m_programmeView, 2, 1, Qt::AlignTop);
    m_layout->setRowStretch(2, 1);

    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(onPlayerStateChanged(QMediaPlayer::State)));
    connect(m_tabBar, SIGNAL(currentChanged(int)), this, SLOT(setCurrentView(int)));
    connect(m_screenShotAction, SIGNAL(triggered()), this, SLOT(showScreenShot()));

    addAction(m_screenShotAction);
    showRemoteView();
}

void RemoteWidget::showRemoteView() {
    if (!m_remoteView) {
        m_remoteView = new RemoteView(m_remoteId, m_stack);
        m_stack->addWidget(m_remoteView);
    }

    m_stack->setCurrentWidget(m_remoteView);
}

void RemoteWidget::showServiceView() {
    if (!m_serviceView) {
        m_serviceView = new ServiceView(m_remoteId, m_stack);
        m_serviceView->reload();
        m_stack->addWidget(m_serviceView);
    }

    m_stack->setCurrentWidget(m_serviceView);
}

void RemoteWidget::showTimerView() {
    if (!m_timerView) {
        m_timerView = new ProgrammeTimerView(m_remoteId, m_stack);
        m_timerView->reload();
        m_stack->addWidget(m_timerView);
    }

    m_stack->setCurrentWidget(m_timerView);
}

void RemoteWidget::setCurrentView(int index) {
    switch (index) {
    case 0:
        showRemoteView();
        break;
    case 1:
        showServiceView();
        break;
    case 2:
        showTimerView();
        break;
    default:
        break;
    }
}

void RemoteWidget::showScreenShot() {
    ScreenShotDialog dialog(m_remoteId, this);
    dialog.reload();
    dialog.exec();
}

void RemoteWidget::updateProgrammeView() {    
    if ((m_programmes) && (m_programmes->status() == Request::Ready) && (!m_programmes->isEmpty())) {
        m_programmeView->setTitle(m_programmes->at(0)->title());
        m_programmeView->setChannelTitle(m_programmes->at(0)->channelTitle());
        m_programmeView->setStartTime(m_programmes->at(0)->startTime());
        m_programmeView->setEndTime(m_programmes->at(0)->endTime());
        m_programmeView->setDescription(m_programmes->at(0)->description());
        m_programmeView->show();
    }
}

void RemoteWidget::onPlayerStateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::PlayingState) {
        if (!m_programmes) {
            m_programmes = Instantiator::createProgrammeList(m_remoteId, this);
            connect(m_programmes, SIGNAL(finished()), this, SLOT(updateProgrammeView()));
        }
            
        m_programmes->reload(QString());
    }
}

