/*
 * Copyright (C) 2015 Stuart Howarth <showarth@marxoft.co.uk>
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

#include "videoplayer.h"
#include <QAction>
#include <QLabel>
#include <QSlider>
#include <QTime>
#include <QToolBar>
#include <QVBoxLayout>
#include <QVideoWidget>

QString formatMSecs(qint64 ms) {
    return ms > 0 ? QTime(0, 0).addMSecs(ms).toString("HH:mm:ss") : QString("00:00:00");
}

VideoPlayer::VideoPlayer(QWidget *parent) :
    QWidget(parent),
    m_player(new QMediaPlayer(this, QMediaPlayer::VideoSurface)),
    m_videoWidget(new QVideoWidget(this)),
    m_toolBar(new QToolBar(this)),
    m_positionSlider(new QSlider(Qt::Horizontal, this)),
    m_volumeSlider(new QSlider(Qt::Horizontal, this)),
    m_positionLabel(new QLabel("00:00:00/00:00:00", this)),
    m_playPauseAction(new QAction(QIcon::fromTheme("media-playback-start"), tr("Play/Pause"), this)),
    m_stopAction(new QAction(QIcon::fromTheme("media-playback-stop"), tr("Stop"), this)),
    m_muteAction(new QAction(QIcon::fromTheme("audio-volume-high"), tr("Mute"), this)),
    m_fullScreenAction(new QAction(QIcon::fromTheme("view-fullscreen"), tr("Toggle fullscreen"), this)),
    m_layout(new QVBoxLayout(this))
{
    m_player->setVideoOutput(m_videoWidget);

    m_videoWidget->addAction(m_playPauseAction);
    m_videoWidget->addAction(m_stopAction);
    m_videoWidget->addAction(m_muteAction);
    m_videoWidget->addAction(m_fullScreenAction);

    m_toolBar->addAction(m_playPauseAction);
    m_toolBar->addAction(m_stopAction);
    m_toolBar->addSeparator();
    m_toolBar->addWidget(m_positionSlider);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_muteAction);
    m_toolBar->addWidget(m_volumeSlider);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_fullScreenAction);

    m_positionSlider->setEnabled(false);
    
    m_volumeSlider->setFixedWidth(100);
    m_volumeSlider->setValue(m_player->volume());

    m_positionLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_positionLabel->setStyleSheet("background: black");

    m_playPauseAction->setShortcut(Qt::Key_P);
    m_playPauseAction->setEnabled(false);
    m_stopAction->setShortcut(Qt::Key_S);
    m_stopAction->setEnabled(false);
    m_muteAction->setShortcut(Qt::Key_M);
    m_muteAction->setCheckable(true);
    m_fullScreenAction->setShortcut(Qt::Key_F);
    
    m_layout->addWidget(m_videoWidget);
    m_layout->addWidget(m_toolBar);
    m_layout->addWidget(m_positionLabel);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);

    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(onDurationChanged(qint64)));
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(onStateChanged(QMediaPlayer::State)));
    connect(m_player, SIGNAL(volumeChanged(int)), this, SLOT(onVolumeChanged(int)));
    connect(m_player, SIGNAL(mutedChanged(bool)), this, SLOT(onMutedChanged(bool)));
    connect(m_videoWidget, SIGNAL(fullScreenChanged(bool)), this, SIGNAL(videoFullScreenChanged(bool)));
    connect(m_positionSlider, SIGNAL(valueChanged(int)),
            this, SLOT(onPositionSliderValueChanged(int)));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
    connect(m_playPauseAction, SIGNAL(triggered()), this, SLOT(togglePlayback()));
    connect(m_stopAction, SIGNAL(triggered()), this, SLOT(stop()));
    connect(m_muteAction, SIGNAL(triggered(bool)), this, SLOT(setMuted(bool)));
    connect(m_fullScreenAction, SIGNAL(triggered()), this, SLOT(toggleVideoFullScreen()));
}

qint64 VideoPlayer::duration() const {
    return m_player->duration();
}

qint64 VideoPlayer::position() const {
    return m_player->position();
}

void VideoPlayer::setPosition(qint64 p) {
    m_player->setPosition(p);
}

QUrl VideoPlayer::source() const {
    return m_source;
}

void VideoPlayer::setSource(const QUrl &u) {
    m_source = u;
    m_playPauseAction->setEnabled(!u.isEmpty());
}

QMediaPlayer::State VideoPlayer::state() const {
    return m_player->state();
}

int VideoPlayer::volume() const {
    return m_player->volume();
}

void VideoPlayer::setVolume(int v) {
    m_player->setVolume(v);
}

bool VideoPlayer::isMuted() const {
    return m_player->isMuted();
}

void VideoPlayer::setMuted(bool m) {
    m_player->setMuted(m);
}

bool VideoPlayer::videoIsFullScreen() const {
    return m_videoWidget->isFullScreen();
}

void VideoPlayer::setVideoFullScreen(bool fs) {
    m_videoWidget->setFullScreen(fs);
}

void VideoPlayer::toggleVideoFullScreen() {
    setVideoFullScreen(!videoIsFullScreen());
}

void VideoPlayer::play() {
    if (state() == QMediaPlayer::StoppedState) {
        m_player->setMedia(source());
    }
    
    m_player->play();
}

void VideoPlayer::play(const QUrl &u) {
    setSource(u);
    play();
}

void VideoPlayer::pause() {
    m_player->pause();
}

void VideoPlayer::togglePlayback() {
    if (state() == QMediaPlayer::PlayingState) {
        pause();
    }
    else {
        play();
    }
}

void VideoPlayer::stop() {
    m_player->stop();
}

void VideoPlayer::onDurationChanged(qint64 d) {
    m_positionSlider->setEnabled(d > 0);
    m_positionSlider->setMaximum(d);
    m_positionLabel->setText(formatMSecs(position()) + "/" + formatMSecs(d));
    emit durationChanged(d);
}

void VideoPlayer::onPositionChanged(qint64 p) {
    if ((m_positionSlider->isEnabled()) && (!m_positionSlider->isSliderDown())) {
        m_positionSlider->setValue(p);
    }
    
    m_positionLabel->setText(formatMSecs(p) + "/" + formatMSecs(duration()));
    emit positionChanged(p);
}

void VideoPlayer::onPositionSliderValueChanged(int v) {
    setPosition(qint64(v));
}

void VideoPlayer::onStateChanged(QMediaPlayer::State s) {
    if (s == QMediaPlayer::StoppedState) {
        m_player->setMedia(QUrl());
        m_positionSlider->setEnabled(false);
        m_positionLabel->setText("00:00:00/00:00:00");
        m_playPauseAction->setIcon(QIcon::fromTheme("media-playback-start"));
        m_stopAction->setEnabled(false);
        setVideoFullScreen(false);
    }
    else {
        if (s == QMediaPlayer::PausedState) {
            m_playPauseAction->setIcon(QIcon::fromTheme("media-playback-start"));
        }
        else {
            m_playPauseAction->setIcon(QIcon::fromTheme("media-playback-pause"));
        }
        
        m_stopAction->setEnabled(true);
    }
    
    emit stateChanged(s);
}

void VideoPlayer::onVolumeChanged(int v) {
    if (!m_volumeSlider->isSliderDown()) {
        m_volumeSlider->setValue(v);
    }
}

void VideoPlayer::onMutedChanged(bool m) {
    m_muteAction->setChecked(m);
    m_muteAction->setIcon(m ? QIcon::fromTheme("audio-volume-muted")
                            : QIcon::fromTheme("audio-volume-high"));
}
