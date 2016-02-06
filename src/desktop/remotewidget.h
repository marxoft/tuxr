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

#ifndef REMOTEWIDGET_H
#define REMOTEWIDGET_H

#include "videoplayer.h"
#include <QWidget>

class ProgrammeList;
class ProgrammeView;
class ProgrammeTimerView;
class RemoteView;
class ServiceView;
class VideoPlayer;
class QGridLayout;
class QStackedWidget;
class QTabBar;

class RemoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteWidget(const QString &remoteId, QWidget *parent = 0);
    
private:    
    void showRemoteView();
    void showServiceView();
    void showTimerView();
    
private Q_SLOTS:
    void setCurrentView(int index);

    void showScreenShot();

    void updateProgrammeView();

    void onPlayerStateChanged(QMediaPlayer::State state);
    
private:
    ProgrammeList *m_programmes;
    
    RemoteView *m_remoteView;
    ServiceView *m_serviceView;
    ProgrammeTimerView *m_timerView;
    
    VideoPlayer *m_player;
    
    ProgrammeView *m_programmeView;

    QTabBar *m_tabBar;
    QStackedWidget *m_stack;

    QAction *m_screenShotAction;

    QGridLayout *m_layout;
    
    QString m_remoteId;
};

#endif // REMOTEWIDGET_H
