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

#include "programme.h"

Programme::Programme(QObject *parent) :
    QObject(parent)
{
}

Programme::Programme(const QString &id, const QString &title, const QString &description,
                     const QDateTime &startTime, const QDateTime &endTime,
                     const QString &channelId, const QString &channelTitle,
                     QObject *parent) :
    QObject(parent),
    m_id(id),
    m_title(title),
    m_description(description),
    m_startTime(startTime),
    m_endTime(endTime),
    m_channelId(channelId),
    m_channelTitle(channelTitle)
{
}

QString Programme::id() const {
    return m_id;
}

void Programme::setId(const QString &i) {
    if (i != id()) {
        m_id = i;
        emit idChanged();
    }
}

QString Programme::title() const {
    return m_title;
}

void Programme::setTitle(const QString &t) {
    if (t != title()) {
        m_title = t;
        emit titleChanged();
    }
}

QString Programme::description() const {
    return m_description;
}

void Programme::setDescription(const QString &d) {
    if (d != description()) {
        m_description = d;
        emit descriptionChanged();
    }
}

QDateTime Programme::startTime() const {
    return m_startTime;
}

void Programme::setStartTime(const QDateTime &dt) {
    if (dt != startTime()) {
        m_startTime = dt;
        emit timeChanged();
    }
}

QDateTime Programme::endTime() const {
    return m_endTime;
}

void Programme::setEndTime(const QDateTime &dt) {
    if (dt != endTime()) {
        m_endTime = dt;
        emit timeChanged();
    }
}

int Programme::duration() const {
    const int secs = startTime().secsTo(endTime());

    if (secs > 60) {
        return secs / 60;
    }

    return 0;
}

QString Programme::channelId() const {
    return m_channelId;
}

void Programme::setChannelId(const QString &i) {
    if (i != channelId()) {
        m_channelId = i;
        emit channelIdChanged();
    }
}

QString Programme::channelTitle() const {
    return m_channelTitle;
}

void Programme::setChannelTitle(const QString &t) {
    if (t != channelTitle()) {
        m_channelTitle = t;
        emit channelTitleChanged();
    }
}
