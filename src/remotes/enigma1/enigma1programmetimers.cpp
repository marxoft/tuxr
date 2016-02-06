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

#include "enigma1programmetimers.h"
#include "programmetimer.h"

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

static QString programmeTimerToQueryString(const ProgrammeTimer *timer) {
    QString query;
    query.append("ref=" + timer->channelId());
    query.append("&channel=" + timer->channelTitle());
    query.append("&descr=" + timer->description());
    query.append(timer->startTime().time().toString("'&shour='H'&smin='m'&sampm='a").remove("."));
    query.append(timer->endTime().time().toString("'&ehour='H'&emin='m'&eampm='a").remove("."));
    
    if (timer->eventType() == ProgrammeTimer::RecurringEvent) {
        query.append("&timer=repeating");
        query.append("&type=repeating");

        if (timer->recurringDays() & ProgrammeTimer::Monday) {
            query.append("&mo=on");
        }
        else {
            query.append("&mo=off");
        }

        if (timer->recurringDays() & ProgrammeTimer::Tuesday) {
            query.append("&tu=on");
        }
        else {
            query.append("&tu=off");
        }

        if (timer->recurringDays() & ProgrammeTimer::Wednesday) {
            query.append("&we=on");
        }
        else {
            query.append("&we=off");
        }

        if (timer->recurringDays() & ProgrammeTimer::Thursday) {
            query.append("&th=on");
        }
        else {
            query.append("&th=off");
        }

        if (timer->recurringDays() & ProgrammeTimer::Friday) {
            query.append("&fr=on");
        }
        else {
            query.append("&fr=off");
        }

        if (timer->recurringDays() & ProgrammeTimer::Saturday) {
            query.append("&sa=on");
        }
        else {
            query.append("&sa=off");
        }

        if (timer->recurringDays() & ProgrammeTimer::Sunday) {
            query.append("&su=on");
        }
        else {
            query.append("&su=off");
        }
    }
    else {
        query.append("&timer=regular");
        query.append("&type=regular");
        query.append(timer->startTime().date().toString("'&sday='d'&smonth='M'&syear='yyyy"));
        query.append(timer->endTime().date().toString("'&eday='d'&emonth='M'&eyear='yyyy"));
    }

    switch (timer->action()) {
    case ProgrammeTimer::Dvr:
        query.append("&action=dvr");
        break;
    case ProgrammeTimer::Ngrab:
        query.append("&action=ngrab");
        break;
    default:
        query.append("&action=zap");
        break;
    }

    switch (timer->afterAction()) {
    case ProgrammeTimer::Standby:
        query.append("&after_event=" + QString::number(AFTER_ACTION_STANDBY));
        break;
    default:
        query.append("&after_event=" + QString::number(AFTER_ACTION_NONE));
        break;
    }

    return query;
}

static int programmeTimerToTypeInteger(const ProgrammeTimer *timer) {
    int type = 0;

    if (timer->eventType() == ProgrammeTimer::RecurringEvent) {
        type += EVENT_TYPE_RECURRING;

        if (timer->recurringDays() & ProgrammeTimer::Monday) {
            type += RECURRING_DAY_MONDAY;
        }

        if (timer->recurringDays() & ProgrammeTimer::Tuesday) {
            type += RECURRING_DAY_TUESDAY;
        }

        if (timer->recurringDays() & ProgrammeTimer::Wednesday) {
            type += RECURRING_DAY_WEDNESDAY;
        }

        if (timer->recurringDays() & ProgrammeTimer::Thursday) {
            type += RECURRING_DAY_THURSDAY;
        }

        if (timer->recurringDays() & ProgrammeTimer::Friday) {
            type += RECURRING_DAY_FRIDAY;
        }

        if (timer->recurringDays() & ProgrammeTimer::Saturday) {
            type += RECURRING_DAY_SATURDAY;
        }

        if (timer->recurringDays() & ProgrammeTimer::Sunday) {
            type += RECURRING_DAY_SUNDAY;
        }
    }
    else {
        type += EVENT_TYPE_ONETIME;
    }

    switch (timer->action()) {
    case ProgrammeTimer::Dvr:
        type += ACTION_DVR;
        break;
    case ProgrammeTimer::Ngrab:
        type += ACTION_NGRAB;
        break;
    default:
        type += ACTION_ZAP;
        break;
    }

    if (timer->afterAction() == ProgrammeTimer::Standby) {
        type += AFTER_ACTION_STANDBY;
    }
    else {
        type += AFTER_ACTION_NONE;
    }

    return type;
}

Enigma1ProgrammeTimers::Enigma1ProgrammeTimers(QObject *parent) :
    ProgrammeTimers(parent),
    m_request(0),
    m_status(Request::Null)
{
}

QString Enigma1ProgrammeTimers::address() const {
    return m_address;
}

void Enigma1ProgrammeTimers::setAddress(const QString &a) {
    if (a != address()) {
        m_address = a;
        emit addressChanged();
    }
}

QString Enigma1ProgrammeTimers::username() const {
    return m_username;
}

void Enigma1ProgrammeTimers::setUsername(const QString &u) {
    if (u != username()) {
        m_username = u;
        emit usernameChanged();
    }
}

QString Enigma1ProgrammeTimers::password() const {
    return m_password;
}

void Enigma1ProgrammeTimers::setPassword(const QString &p) {
    if (p != password()) {
        m_password = p;
        emit passwordChanged();
    }
}

QString Enigma1ProgrammeTimers::errorString() const {
    return m_errorString;
}

void Enigma1ProgrammeTimers::setErrorString(const QString &e) {
    m_errorString = e;
}

Request::Status Enigma1ProgrammeTimers::status() const {
    return m_status;
}

void Enigma1ProgrammeTimers::setStatus(Request::Status s) {
    if (s != status()) {
        m_status = s;
        emit statusChanged(s);
    }
}

void Enigma1ProgrammeTimers::addProgrammeTimer(ProgrammeTimer *timer) {
    if (status() == Request::Active) {
        return;
    }    

    send("/addTimerEvent?" + programmeTimerToQueryString(timer));
}

void Enigma1ProgrammeTimers::removeProgrammeTimer(ProgrammeTimer *timer) {
    if (status() == Request::Active) {
        return;
    }

    QString command("/deleteTimerEvent?");
    command.append("ref=" + timer->channelId());

    int secs = timer->startTime().toTime_t();

    if ((timer->eventType() == ProgrammeTimer::RecurringEvent) && (secs > 86400)) {
        // Get seconds since start of day.
        secs = secs % 86400;
    }

    command.append("&start=" + QString::number(secs));
    command.append("&type=" + QString::number(programmeTimerToTypeInteger(timer)));
    command.append("&force=no");
    send(command);
}

void Enigma1ProgrammeTimers::updateProgrammeTimer(ProgrammeTimer *oldTimer, ProgrammeTimer *newTimer) {
    if (status() == Request::Active) {
        return;
    }

    QString command("/changeTimerEvent?");
    command.append("old_type=" + QString::number(programmeTimerToTypeInteger(oldTimer)));
    
    int secs = oldTimer->startTime().toTime_t();

    if ((oldTimer->eventType() == ProgrammeTimer::RecurringEvent) && (secs > 86400)) {
        // Get seconds since start of day.
        secs = secs % 86400;
    }

    command.append("&old_stime=" + QString::number(secs));
    command.append("&force=no");
    command.append("&" + programmeTimerToQueryString(newTimer));
    send(command);
}

void Enigma1ProgrammeTimers::cleanupProgrammeTimers() {
    if (status() == Request::Active) {
        return;
    }

    send("/cleanupTimerList");
}

void Enigma1ProgrammeTimers::clearProgrammeTimers() {
    if (status() == Request::Active) {
        return;
    }

    send("/clearTimerList");
}

void Enigma1ProgrammeTimers::send(const QString &command) {
    setStatus(Request::Active);
    
    if (address().isEmpty()) {
        setErrorString(tr("IP address is empty."));
        setStatus(Request::Error);
        emit finished();
        return;
    }
    
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

    m_request->get(QUrl::fromUserInput(address() + command));
}

void Enigma1ProgrammeTimers::cancel() {
    if (m_request) {
        m_request->cancel();
    }
}

void Enigma1ProgrammeTimers::onRequestFinished() {
    if (!m_request) {
        return;
    }

    setErrorString(m_request->errorString());
    setStatus(m_request->status());
    emit finished();
}
