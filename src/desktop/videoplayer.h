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

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMediaPlayer>
#include <QWidget>

class QLabel;
class QSlider;
class QToolBar;
class QVBoxLayout;
class QVideoWidget;

class VideoPlayer : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource)
    Q_PROPERTY(QMediaPlayer::State state READ state NOTIFY stateChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(bool videoFullScreen READ videoIsFullScreen WRITE setVideoFullScreen
               NOTIFY videoFullScreenChanged)
    
public:
    explicit VideoPlayer(QWidget *parent = 0);

    qint64 duration() const;

    qint64 position() const;
    
    QUrl source() const;
    void setSource(const QUrl &u);

    QMediaPlayer::State state() const;

    int volume() const;
    bool isMuted() const;

    bool videoIsFullScreen() const;
    
public Q_SLOTS:
    void setPosition(qint64 p);
    
    void setVolume(int v);
    void setMuted(bool m);

    void setVideoFullScreen(bool fs);
    void toggleVideoFullScreen();
    
    void play();
    void play(const QUrl &u);
    void pause();
    void togglePlayback();
    void stop();

private Q_SLOTS:
    void onPositionChanged(qint64 p);
    void onDurationChanged(qint64 d);
    void onPositionSliderValueChanged(int v);
    void onStateChanged(QMediaPlayer::State s);
    void onVolumeChanged(int v);
    void onMutedChanged(bool m);

Q_SIGNALS:
    void durationChanged(qint64 d);
    void positionChanged(qint64 p);
    void stateChanged(QMediaPlayer::State s);
    void volumeChanged(int v);
    void mutedChanged(bool m);
    void videoFullScreenChanged(bool fs);
    
private:
    QMediaPlayer *m_player;

    QVideoWidget *m_videoWidget;
    QToolBar *m_toolBar;
    QSlider *m_positionSlider;
    QSlider *m_volumeSlider;
    QLabel *m_positionLabel;

    QAction *m_playPauseAction;
    QAction *m_stopAction;
    QAction *m_muteAction;
    QAction *m_fullScreenAction;

    QVBoxLayout *m_layout;

    QUrl m_source;
};

#endif // VIDEOPLAYER_H
