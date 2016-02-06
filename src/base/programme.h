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

#ifndef PROGRAMME_H
#define PROGRAMME_H

#include <QObject>
#include <QDateTime>

class Programme : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime NOTIFY timeChanged)
    Q_PROPERTY(QDateTime endTime READ endTime WRITE setEndTime NOTIFY timeChanged)
    Q_PROPERTY(int duration READ duration NOTIFY timeChanged)
    Q_PROPERTY(QString channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)
    Q_PROPERTY(QString channelTitle READ channelTitle WRITE setChannelTitle NOTIFY channelTitleChanged)

public:
    explicit Programme(QObject *parent = 0);
    explicit Programme(const QString &id, const QString &title, const QString &description,
                       const QDateTime &startTime, const QDateTime &endTime,
                       const QString &channelId, const QString &channelTitle,
                       QObject *parent = 0);

    QString id() const;
    void setId(const QString &i);

    QString title() const;
    void setTitle(const QString &t);
    
    QString description() const;
    void setDescription(const QString &d);

    QDateTime startTime() const;
    void setStartTime(const QDateTime &dt);
    
    QDateTime endTime() const;
    void setEndTime(const QDateTime &dt);

    int duration() const;

    QString channelId() const;
    void setChannelId(const QString &i);
    
    QString channelTitle() const;
    void setChannelTitle(const QString &t);

Q_SIGNALS:
    void idChanged();
    void titleChanged();
    void descriptionChanged();
    void timeChanged();
    void channelIdChanged();
    void channelTitleChanged();

private:
    QString m_id;
    QString m_title;
    QString m_description;

    QDateTime m_startTime;
    QDateTime m_endTime;

    QString m_channelId;
    QString m_channelTitle;
};

#endif // PROGRAMME_H
