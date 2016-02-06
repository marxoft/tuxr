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

#include "programmetimermodel.h"
#include "programmetimer.h"
#include <QFont>

ProgrammeTimerModel::ProgrammeTimerModel(QObject *parent) :
    QAbstractListModel(parent),
    m_timers(0)
{
    m_roles[Qt::DisplayRole] = "display";
    m_roles[IdRole] = "id";
    m_roles[DescriptionRole] = "description";
    m_roles[StartTimeRole] = "startTime";
    m_roles[EndTimeRole] = "endTime";
    m_roles[DurationRole] = "duration";
    m_roles[ChannelIdRole] = "channelId";
    m_roles[ChannelTitleRole] = "channelTitle";
    m_roles[EventTypeRole] = "eventType";
    m_roles[RecurringDaysRole] = "recurringDays";
    m_roles[ActionRole] = "action";
    m_roles[AfterActionRole] = "afterAction";
#if QT_VERSION < 0x050000
    setRoleNames(m_roles);
#endif
}

ProgrammeTimerList* ProgrammeTimerModel::timers() const {
    return m_timers;
}

Request::Status ProgrammeTimerModel::status() const {
    return m_timers ? m_timers->status() : Request::Null;
}

QString ProgrammeTimerModel::errorString() const {
    return m_timers ? m_timers->errorString() : QString();
}

void ProgrammeTimerModel::setTimers(ProgrammeTimerList *t) {
    if (t == m_timers) {
        return;
    }

    beginResetModel();
    
    if (m_timers) {
        disconnect(m_timers, SIGNAL(finished()), this, SLOT(onFinished()));
        disconnect(m_timers, SIGNAL(countChanged(int)), this, SIGNAL(countChanged(int)));
        disconnect(m_timers, SIGNAL(statusChanged(Request::Status)),
                   this, SIGNAL(statusChanged(Request::Status)));
    }

    m_timers = t;

    if (m_timers) {
        connect(m_timers, SIGNAL(finished()), this, SLOT(onFinished()));
        connect(m_timers, SIGNAL(countChanged(int)), this, SIGNAL(countChanged(int)));
        connect(m_timers, SIGNAL(statusChanged(Request::Status)),
                this, SIGNAL(statusChanged(Request::Status)));
    }

    endResetModel();
    emit timersChanged();
}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> ProgrammeTimerModel::roleNames() const {
    return m_roles;
}
#endif

int ProgrammeTimerModel::rowCount(const QModelIndex &) const {
    return m_timers ? m_timers->count() : 0;
}

int ProgrammeTimerModel::columnCount(const QModelIndex &) const {
    return 4;
}

QVariant ProgrammeTimerModel::data(const QModelIndex &index, int role) const {
    if (!m_timers) {
        return QVariant();
    }
    
    const ProgrammeTimer *timer = m_timers->at(index.row());

    if (!timer) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return timer->channelTitle();
        case 1:
            if (timer->eventType() == ProgrammeTimer::RecurringEvent) {
                return timer->startTime().time();
            }

            return timer->startTime();
        case 2:
            if (timer->eventType() == ProgrammeTimer::RecurringEvent) {
                return timer->endTime().time();
            }

            return timer->endTime();
        case 3:
            return timer->description();
        default:
            break;
        }

        break;
    case IdRole:
        return timer->id();
    case DescriptionRole:
        return timer->description();
    case StartTimeRole:
        return timer->startTime();
    case EndTimeRole:
        return timer->endTime();
    case DurationRole:
        return timer->duration();
    case ChannelIdRole:
        return timer->channelId();
    case ChannelTitleRole:
        return timer->channelTitle();
    case EventTypeRole:
        return timer->eventType();
    case RecurringDaysRole:
        return int(timer->recurringDays());
    case ActionRole:
        return timer->action();
    case AfterActionRole:
        return timer->afterAction();
    default:
        break;
    }

    return QVariant();
}

QVariant ProgrammeTimerModel::data(int row, const QByteArray &role) const {
    return data(index(row), m_roles.key(role));
}

bool ProgrammeTimerModel::setData(const QModelIndex &index, const QVariant &data, int role) {
    if (!m_timers) {
        return false;
    }

    ProgrammeTimer *timer = m_timers->at(index.row());

    if (!timer) {
        return false;
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            timer->setChannelTitle(data.toString());
            break;
        case 1:
            timer->setStartTime(data.toDateTime());
            break;
        case 2:
            timer->setEndTime(data.toDateTime());
            break;
        case 3:
            timer->setDescription(data.toString());
            break;
        default:
            return false;
        }

        break;
    case IdRole:
        timer->setId(data.toString());
        break;
    case StartTimeRole:
        timer->setStartTime(data.toDateTime());
        break;
    case EndTimeRole:
        timer->setEndTime(data.toDateTime());
        break;
    case ChannelIdRole:
        timer->setChannelId(data.toString());
        break;
    case ChannelTitleRole:
        timer->setChannelTitle(data.toString());
        break;
    case EventTypeRole:
        timer->setEventType(ProgrammeTimer::EventType(data.toInt()));
        break;
    case RecurringDaysRole:
        timer->setRecurringDays(ProgrammeTimer::Days(data.toInt()));
        break;
    case ActionRole:
        timer->setAction(ProgrammeTimer::Action(data.toInt()));
        break;
    case AfterActionRole:
        timer->setAfterAction(ProgrammeTimer::Action(data.toInt()));
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

bool ProgrammeTimerModel::setData(int row, const QVariant &data, const QByteArray &role) {
    return setData(index(row), data, m_roles.key(role));
}

QVariant ProgrammeTimerModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal) {
        switch (role) {
        case Qt::DisplayRole:
            switch (section) {
            case 0:
                return tr("Channel");
            case 1:
                return tr("Start");
            case 2:
                return tr("End");
            case 3:
                return tr("Description");
            default:
                break;
            }

            break;
        case Qt::FontRole: {
            QFont font;
            font.setBold(true);
            return font;
        }
        default:
            break;
        }
    }

    return QVariant();
}

QMap<int, QVariant> ProgrammeTimerModel::itemData(const QModelIndex &index) const {
    QMap<int, QVariant> map;

    if (!m_timers) {
        return map;
    }
    
    const ProgrammeTimer *timer = m_timers->at(index.row());
    
    if (!timer) {
        return map;
    }
    
    map[IdRole] = timer->id();
    map[DescriptionRole] = timer->description();
    map[StartTimeRole] = timer->startTime();
    map[EndTimeRole] = timer->endTime();
    map[DurationRole] = timer->duration();
    map[ChannelIdRole] = timer->channelId();
    map[ChannelTitleRole] = timer->channelTitle();
    map[EventTypeRole] = timer->eventType();
    map[RecurringDaysRole] = int(timer->recurringDays());
    map[ActionRole] = timer->action();
    map[AfterActionRole] = timer->afterAction();
    return map;
}

QVariantMap ProgrammeTimerModel::itemData(int row) const {
    QVariantMap map;

    if (!m_timers) {
        return map;
    }
    
    const ProgrammeTimer *timer = m_timers->at(row);

    if (!timer) {
        return map;
    }

    map["id"] = timer->id();
    map["description"] = timer->description();
    map["startTime"] = timer->startTime();
    map["endTime"] = timer->endTime();
    map["duration"] = timer->duration();
    map["channelId"] = timer->channelId();
    map["channelTitle"] = timer->channelTitle();
    map["eventType"] = timer->eventType();
    map["recurringDays"] = int(timer->recurringDays());
    map["action"] = timer->action();
    map["afterAction"] = timer->afterAction();
    return map;
}

bool ProgrammeTimerModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &data) {
    if (data.isEmpty()) {
        return false;
    }
    
    QMapIterator<int, QVariant> iterator(data);

    while (iterator.hasNext()) {
        iterator.next();

        if (!setData(index, iterator.value(), iterator.key())) {
            return false;
        }
    }

    return true;
}

bool ProgrammeTimerModel::setItemData(int row, const QVariantMap &data) {
    if (data.isEmpty()) {
        return false;
    }
    
    QMapIterator<QString, QVariant> iterator(data);

    while (iterator.hasNext()) {
        iterator.next();

        if (!setData(row, iterator.value(), iterator.key().toUtf8())) {
            return false;
        }
    }

    return true;
}

QModelIndexList ProgrammeTimerModel::match(const QModelIndex &start, int role, const QVariant &value, int hits,
                                           Qt::MatchFlags flags) const {
    return QAbstractListModel::match(start, role, value, hits, flags);
}

int ProgrammeTimerModel::match(int start, const QByteArray &role, const QVariant &value, int flags) const {
    const QModelIndexList idxs = match(index(start), m_roles.key(role), value, 1, Qt::MatchFlags(flags));
    return idxs.isEmpty() ? -1 : idxs.first().row();
}

void ProgrammeTimerModel::clear() {
    if ((m_timers) && (!m_timers->isEmpty())) {
	beginResetModel();
	m_timers->clear();
	endResetModel();
    }
}

void ProgrammeTimerModel::reload() {
    if (m_timers) {
        beginResetModel();
        m_timers->reload();
        endResetModel();
    }
}

void ProgrammeTimerModel::cancel() {
    if (m_timers) {
        m_timers->cancel();
    }
}

void ProgrammeTimerModel::onFinished() {
    if ((m_timers) && (m_timers->status() == Request::Ready) && (!m_timers->isEmpty())) {
        beginResetModel();
        endResetModel();
    }
}
