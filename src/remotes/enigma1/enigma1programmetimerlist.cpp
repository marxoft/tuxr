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

#include "enigma1programmetimerlist.h"
#include "programmetimer.h"
#include <QStringList>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

static const int EVENT_TYPE_ONETIME = 0;
static const int EVENT_TYPE_RECURRING = 262144;

static const int ACTION_DVR = 44;
static const int ACTION_ZAP = 34;
static const int ACTION_NGRAB = 131108;

static const int AFTER_ACTION_NONE = 0;
static const int AFTER_ACTION_STANDBY = 134217728;

static const int RECURRING_DAY_MONDAY = 1048576;
static const int RECURRING_DAY_TUESDAY = 2097152;
static const int RECURRING_DAY_WEDNESDAY = 4194304;
static const int RECURRING_DAY_THURSDAY = 8388608;
static const int RECURRING_DAY_FRIDAY = 16777216;
static const int RECURRING_DAY_SATURDAY = 33554432;
static const int RECURRING_DAY_SUNDAY = 524288;

Enigma1ProgrammeTimerList::Enigma1ProgrammeTimerList(QObject *parent) :
    ProgrammeTimerList(parent),
    m_request(0),
    m_status(Request::Null)
{
}

QString Enigma1ProgrammeTimerList::address() const {
    return m_address;
}

void Enigma1ProgrammeTimerList::setAddress(const QString &a) {
    if (a != address()) {
        m_address = a;
        emit addressChanged();
    }
}

QString Enigma1ProgrammeTimerList::username() const {
    return m_username;
}

void Enigma1ProgrammeTimerList::setUsername(const QString &u) {
    if (u != username()) {
        m_username = u;
        emit usernameChanged();
    }
}

QString Enigma1ProgrammeTimerList::password() const {
    return m_password;
}

void Enigma1ProgrammeTimerList::setPassword(const QString &p) {
    if (p != password()) {
        m_password = p;
        emit passwordChanged();
    }
}

QString Enigma1ProgrammeTimerList::errorString() const {
    return m_errorString;
}

void Enigma1ProgrammeTimerList::setErrorString(const QString &e) {
    m_errorString = e;
}

Request::Status Enigma1ProgrammeTimerList::status() const {
    return m_status;
}

void Enigma1ProgrammeTimerList::setStatus(Request::Status s) {
    if (s != status()) {
        m_status = s;
        emit statusChanged(s);
    }
}

int Enigma1ProgrammeTimerList::count() const {
    return m_timers.size();
}

ProgrammeTimer* Enigma1ProgrammeTimerList::at(int i) const {
    if ((i >= 0) && (i < m_timers.size())) {
        return m_timers.at(i);
    }

    return 0;
}

void Enigma1ProgrammeTimerList::clear() {
    if (!m_timers.isEmpty()) {
        qDeleteAll(m_timers);
        m_timers.clear();
        emit countChanged(0);
    }
}

void Enigma1ProgrammeTimerList::reload() {
    setStatus(Request::Active);
    
    if (address().isEmpty()) {
        setErrorString(tr("IP address is empty."));
        setStatus(Request::Error);
        emit finished();
        return;
    }

    clear();
    
    if (!m_request) {
        m_request = new Request(this);
        connect(m_request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    }
    
    if ((!username().isEmpty()) && (!password().isEmpty())) {
        QVariantMap headers;
        headers["Authorization"] = "Basic " + QString("%1:%2").arg(username())
                                                     .arg(password()).toUtf8().toBase64();
        m_request->setHeaders(headers);
    }

    m_request->get(QUrl::fromUserInput(address() + "/body?mode=timers"));
}

void Enigma1ProgrammeTimerList::cancel() {
    if (m_request) {
        m_request->cancel();
    }
}

void Enigma1ProgrammeTimerList::onRequestFinished() {
    if (!m_request) {
        return;
    }

    if (m_request->status() == Request::Ready) {
	const QStringList results =
        QString::fromUtf8(m_request->result()).split("editLink = escape('", QString::SkipEmptyParts);

        for (int i = 1; i < results.size(); i++) {
            QString result = results.at(i);
            result = result.left(result.indexOf("')"));
            const QUrl url = QUrl::fromUserInput(address() + "?" + result);

            if (url.isValid()) {
                ProgrammeTimer *timer = new ProgrammeTimer(this);
#if QT_VERSION >= 0x050000
                QUrlQuery query(url);
                timer->setId(result);
                timer->setDescription(query.queryItemValue("descr"));
                timer->setChannelId(query.queryItemValue("ref"));
                timer->setChannelTitle(query.queryItemValue("channel"));
                int type = query.queryItemValue("type").toInt();

                if (type > AFTER_ACTION_STANDBY) {
                    timer->setAfterAction(ProgrammeTimer::Standby);
                    type -= AFTER_ACTION_STANDBY;
                }
                else {
                    timer->setAfterAction(ProgrammeTimer::None);
                    type -= AFTER_ACTION_NONE;
                }

                if (type > EVENT_TYPE_RECURRING) {
                    timer->setEventType(ProgrammeTimer::RecurringEvent);
                    timer->setStartTime(QDateTime(QDate::currentDate())
                                        .addSecs(query.queryItemValue("start").toLongLong()));
                    int days = type - type % EVENT_TYPE_RECURRING;
                    type -= days;
                    ProgrammeTimer::Days recurringDays;

                    if (days >= RECURRING_DAY_SATURDAY) {
                        recurringDays |= ProgrammeTimer::Saturday;
                        days -= RECURRING_DAY_SATURDAY;
                    }

                    if (days >= RECURRING_DAY_FRIDAY) {
                        recurringDays |= ProgrammeTimer::Friday;
                        days -= RECURRING_DAY_FRIDAY;
                    }

                    if (days >= RECURRING_DAY_THURSDAY) {
                        recurringDays |= ProgrammeTimer::Thursday;
                        days -= RECURRING_DAY_THURSDAY;
                    }

                    if (days >= RECURRING_DAY_WEDNESDAY) {
                        recurringDays |= ProgrammeTimer::Wednesday;
                        days -= RECURRING_DAY_WEDNESDAY;
                    }

                    if (days >= RECURRING_DAY_TUESDAY) {
                        recurringDays |= ProgrammeTimer::Tuesday;
                        days -= RECURRING_DAY_TUESDAY;
                    }

                    if (days >= RECURRING_DAY_MONDAY) {
                        recurringDays |= ProgrammeTimer::Monday;
                        days -= RECURRING_DAY_MONDAY;
                    }

                    if (days >= RECURRING_DAY_SUNDAY) {
                        recurringDays |= ProgrammeTimer::Sunday;
                        days -= RECURRING_DAY_SUNDAY;
                    }

                    timer->setRecurringDays(recurringDays);
                }
                else {
                    timer->setEventType(ProgrammeTimer::OnetimeEvent);
                    timer->setStartTime(QDateTime::fromTime_t(query.queryItemValue("start").toUInt()));
                }

                timer->setEndTime(timer->startTime().addSecs(query.queryItemValue("duration").toLongLong()));

                switch (type) {
                case ACTION_DVR:
                    timer->setAction(ProgrammeTimer::Dvr);
                    break;
                case ACTION_NGRAB:
                    timer->setAction(ProgrammeTimer::Ngrab);
                    break;
                default:
                    timer->setAction(ProgrammeTimer::Zap);
                    break;
                }
#else
                timer->setId(result);
                timer->setDescription(url.queryItemValue("descr"));
                timer->setChannelId(url.queryItemValue("ref"));
                timer->setChannelTitle(url.queryItemValue("channel"));
                int type = url.queryItemValue("type").toInt();

                if (type > AFTER_ACTION_STANDBY) {
                    timer->setAfterAction(ProgrammeTimer::Standby);
                    type -= AFTER_ACTION_STANDBY;
                }
                else {
                    timer->setAfterAction(ProgrammeTimer::None);
                }

                if (type > EVENT_TYPE_RECURRING) {
                    timer->setEventType(ProgrammeTimer::RecurringEvent);
                    timer->setStartTime(QDateTime(QDate::currentDate())
                                        .addSecs(url.queryItemValue("start").toLongLong()));
                    int days = type - type % EVENT_TYPE_RECURRING;
                    type -= days;
                    ProgrammeTimer::Days recurringDays;

                    if (days >= RECURRING_DAY_SATURDAY) {
                        recurringDays |= ProgrammeTimer::Saturday;
                        days -= RECURRING_DAY_SATURDAY;
                    }

                    if (days >= RECURRING_DAY_FRIDAY) {
                        recurringDays |= ProgrammeTimer::Friday;
                        days -= RECURRING_DAY_FRIDAY;
                    }

                    if (days >= RECURRING_DAY_THURSDAY) {
                        recurringDays |= ProgrammeTimer::Thursday;
                        days -= RECURRING_DAY_THURSDAY;
                    }

                    if (days >= RECURRING_DAY_WEDNESDAY) {
                        recurringDays |= ProgrammeTimer::Wednesday;
                        days -= RECURRING_DAY_WEDNESDAY;
                    }

                    if (days >= RECURRING_DAY_TUESDAY) {
                        recurringDays |= ProgrammeTimer::Tuesday;
                        days -= RECURRING_DAY_TUESDAY;
                    }

                    if (days >= RECURRING_DAY_MONDAY) {
                        recurringDays |= ProgrammeTimer::Monday;
                        days -= RECURRING_DAY_MONDAY;
                    }

                    if (days >= RECURRING_DAY_SUNDAY) {
                        recurringDays |= ProgrammeTimer::Sunday;
                        days -= RECURRING_DAY_SUNDAY;
                    }
                    
                    timer->setRecurringDays(recurringDays);
                }
                else {
                    timer->setEventType(ProgrammeTimer::OnetimeEvent);
                    timer->setStartTime(QDateTime::fromTime_t(url.queryItemValue("start").toUInt()));
                }

                timer->setEndTime(timer->startTime().addSecs(url.queryItemValue("duration").toLongLong()));

                switch (type) {
                case ACTION_DVR:
                    timer->setAction(ProgrammeTimer::Dvr);
                    break;
                case ACTION_NGRAB:
                    timer->setAction(ProgrammeTimer::Ngrab);
                    break;
                default:
                    timer->setAction(ProgrammeTimer::Zap);
                    break;
                }
#endif                
                m_timers << timer;
            }
        }
        
        emit countChanged(count());
    }

    setErrorString(m_request->errorString());
    setStatus(m_request->status());
    emit finished();
}
