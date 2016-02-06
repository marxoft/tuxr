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

#include "programmetimer.h"

ProgrammeTimer::ProgrammeTimer(QObject *parent) :
    QObject(parent),
    m_eventType(OnetimeEvent),
    m_action(Dvr),
    m_afterAction(None)
{
    const QDateTime current = QDateTime::currentDateTime();
    setStartTime(current);
    setEndTime(current);
}

ProgrammeTimer::ProgrammeTimer(const QString &id, const QString &description,
                               const QDateTime &startTime, const QDateTime &endTime,
                               const QString &channelId, const QString &channelTitle,
                               EventType eventType, Days recurringDays, Action action,
                               Action afterAction, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_description(description),
    m_startTime(startTime),
    m_endTime(endTime),
    m_channelId(channelId),
    m_channelTitle(channelTitle),
    m_eventType(eventType),
    m_recurringDays(recurringDays),
    m_action(action),
    m_afterAction(afterAction)
{
}

QString ProgrammeTimer::id() const {
    return m_id;
}

void ProgrammeTimer::setId(const QString &i) {
    if (i != id()) {
        m_id = i;
        emit idChanged();
    }
}

QString ProgrammeTimer::description() const {
    return m_description;
}

void ProgrammeTimer::setDescription(const QString &d) {
    if (d != description()) {
        m_description = d;
        emit descriptionChanged();
    }
}

QDateTime ProgrammeTimer::startTime() const {
    return m_startTime;
}

void ProgrammeTimer::setStartTime(const QDateTime &dt) {
    if (dt != startTime()) {
        m_startTime = dt;
        emit timeChanged();
    }
}

QDateTime ProgrammeTimer::endTime() const {
    return m_endTime;
}

void ProgrammeTimer::setEndTime(const QDateTime &dt) {
    if (dt != endTime()) {
        m_endTime = dt;
        emit timeChanged();
    }
}

int ProgrammeTimer::duration() const {
    const int secs =
    eventType() == RecurringEvent ? startTime().time().secsTo(endTime().time())
                                  : startTime().secsTo(endTime());

    if (secs > 60) {
        return secs / 60;
    }

    return 0;
}

QString ProgrammeTimer::channelId() const {
    return m_channelId;
}

void ProgrammeTimer::setChannelId(const QString &i) {
    if (i != channelId()) {
        m_channelId = i;
        emit channelIdChanged();
    }
}

QString ProgrammeTimer::channelTitle() const {
    return m_channelTitle;
}

void ProgrammeTimer::setChannelTitle(const QString &t) {
    if (t != channelTitle()) {
        m_channelTitle = t;
        emit channelTitleChanged();
    }
}

ProgrammeTimer::EventType ProgrammeTimer::eventType() const {
    return m_eventType;
}

void ProgrammeTimer::setEventType(ProgrammeTimer::EventType t) {
    if (t != eventType()) {
        m_eventType = t;
        emit eventTypeChanged();
    }
}

ProgrammeTimer::Days ProgrammeTimer::recurringDays() const {
    return m_recurringDays;
}

void ProgrammeTimer::setRecurringDays(ProgrammeTimer::Days d) {
    if (d != recurringDays()) {
        m_recurringDays = d;
        emit recurringDaysChanged();
    }
}

void ProgrammeTimer::resetRecurringDays() {
    setRecurringDays(ProgrammeTimer::Days(0));
}

void ProgrammeTimer::setRecurringDay(int d, bool enabled) {
    if (enabled != testRecurringDay(d)) {
        if (enabled) {
            m_recurringDays |= ProgrammeTimer::Day(d);
        }
        else {
            m_recurringDays &= ~ProgrammeTimer::Day(d);
        }
    
        emit recurringDaysChanged();
    }
}

bool ProgrammeTimer::testRecurringDay(int d) const {
    return m_recurringDays & ProgrammeTimer::Day(d);
}

ProgrammeTimer::Action ProgrammeTimer::action() const {
    return m_action;
}

void ProgrammeTimer::setAction(ProgrammeTimer::Action a) {
    if (a != action()) {
        m_action = a;
        emit actionChanged();
    }
}

ProgrammeTimer::Action ProgrammeTimer::afterAction() const {
    return m_afterAction;
}

void ProgrammeTimer::setAfterAction(ProgrammeTimer::Action a) {
    if (a != afterAction()) {
        m_afterAction = a;
        emit afterActionChanged();
    }
}
