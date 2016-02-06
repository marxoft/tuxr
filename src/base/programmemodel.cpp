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

#include "programmemodel.h"
#include "programme.h"
#include <QFont>

ProgrammeModel::ProgrammeModel(QObject *parent) :
    QAbstractListModel(parent),
    m_programmes(0)
{
    m_roles[Qt::DisplayRole] = "display";
    m_roles[IdRole] = "id";
    m_roles[TitleRole] = "title";
    m_roles[DescriptionRole] = "description";
    m_roles[StartTimeRole] = "startTime";
    m_roles[EndTimeRole] = "endTime";
    m_roles[DurationRole] = "duration";
    m_roles[ChannelIdRole] = "channelId";
    m_roles[ChannelTitleRole] = "channelTitle";
#if QT_VERSION < 0x050000
    setRoleNames(m_roles);
#endif
}

ProgrammeList* ProgrammeModel::programmes() const {
    return m_programmes;
}

void ProgrammeModel::setProgrammes(ProgrammeList *p) {
    if (p == m_programmes) {
        return;
    }

    beginResetModel();

    if (m_programmes) {
        disconnect(m_programmes, SIGNAL(finished()), this, SLOT(onFinished()));
        disconnect(m_programmes, SIGNAL(countChanged(int)), this, SIGNAL(countChanged(int)));
        disconnect(m_programmes, SIGNAL(statusChanged(Request::Status)),
                   this, SIGNAL(statusChanged(Request::Status)));
    }
    
    m_programmes = p;

    if (m_programmes) {
        connect(m_programmes, SIGNAL(finished()), this, SLOT(onFinished()));
        connect(m_programmes, SIGNAL(countChanged(int)), this, SIGNAL(countChanged(int)));
        connect(m_programmes, SIGNAL(statusChanged(Request::Status)),
                this, SIGNAL(statusChanged(Request::Status)));
    }

    endResetModel();
    emit programmesChanged();
}

Request::Status ProgrammeModel::status() const {
    return m_programmes ? m_programmes->status() : Request::Null;
}

QString ProgrammeModel::errorString() const {
    return m_programmes ? m_programmes->errorString() : QString();
}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> ProgrammeModel::roleNames() const {
    return m_roles;
}
#endif

int ProgrammeModel::rowCount(const QModelIndex &) const {
    return m_programmes ? m_programmes->count() : 0;
}

int ProgrammeModel::columnCount(const QModelIndex &) const {
    return 4;
}

QVariant ProgrammeModel::data(const QModelIndex &index, int role) const {
    if (!m_programmes) {
        return QVariant();
    }
    
    const Programme *programme = m_programmes->at(index.row());

    if (!programme) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return programme->channelTitle();
        case 1:
            return programme->startTime();
        case 2:
            return programme->endTime();
        case 3:
            return programme->title();
        default:
            break;
        }

        break;
    case IdRole:
        return programme->id();
    case TitleRole:
        return programme->title();
    case DescriptionRole:
        return programme->description();
    case StartTimeRole:
        return programme->startTime();
    case EndTimeRole:
        return programme->endTime();
    case DurationRole:
        return programme->duration();
    case ChannelIdRole:
        return programme->channelId();
    case ChannelTitleRole:
        return programme->channelTitle();
    default:
        break;
    }

    return QVariant();
}

QVariant ProgrammeModel::data(int row, const QByteArray &role) const {
    return data(index(row), m_roles.key(role));
}

bool ProgrammeModel::setData(const QModelIndex &index, const QVariant &data, int role) {
    if (!m_programmes) {
        return false;
    }

    Programme *programme = m_programmes->at(index.row());

    if (!programme) {
        return false;
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            programme->setChannelTitle(data.toString());
            break;
        case 1:
            programme->setStartTime(data.toDateTime());
            break;
        case 2:
            programme->setEndTime(data.toDateTime());
            break;
        case 3:
            programme->setTitle(data.toString());
            break;
        default:
            return false;
        }

        break;
    case IdRole:
        programme->setId(data.toString());
        break;
    case TitleRole:
        programme->setTitle(data.toString());
        break;
    case DescriptionRole:
        programme->setDescription(data.toString());
        break;
    case StartTimeRole:
        programme->setStartTime(data.toDateTime());
        break;
    case EndTimeRole:
        programme->setEndTime(data.toDateTime());
        break;
    case ChannelIdRole:
        programme->setChannelId(data.toString());
        break;
    case ChannelTitleRole:
        programme->setChannelTitle(data.toString());
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

bool ProgrammeModel::setData(int row, const QVariant &data, const QByteArray &role) {
    return setData(index(row), data, m_roles.key(role));
}

QVariant ProgrammeModel::headerData(int section, Qt::Orientation orientation, int role) const {
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
                return tr("Title");
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

QMap<int, QVariant> ProgrammeModel::itemData(const QModelIndex &index) const {
    QMap<int, QVariant> map;

    if (!m_programmes) {
        return map;
    }
    
    const Programme *programme = m_programmes->at(index.row());
    
    if (!programme) {
        return map;
    }
    
    map[IdRole] = programme->id();
    map[TitleRole] = programme->title();
    map[DescriptionRole] = programme->description();
    map[StartTimeRole] = programme->startTime();
    map[EndTimeRole] = programme->endTime();
    map[DurationRole] = programme->duration();
    map[ChannelIdRole] = programme->channelId();
    map[ChannelTitleRole] = programme->channelTitle();
    return map;
}

QVariantMap ProgrammeModel::itemData(int row) const {
    QVariantMap map;

    if (!m_programmes) {
        return map;
    }
    
    const Programme *programme = m_programmes->at(row);

    if (!programme) {
        return map;
    }

    map["id"] = programme->id();
    map["title"] = programme->title();
    map["description"] = programme->description();
    map["startTime"] = programme->startTime();
    map["endTime"] = programme->endTime();
    map["duration"] = programme->duration();
    map["channelId"] = programme->channelId();
    map["channelTitle"] = programme->channelTitle();
    return map;
}

bool ProgrammeModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &data) {
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

bool ProgrammeModel::setItemData(int row, const QVariantMap &data) {
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

QModelIndexList ProgrammeModel::match(const QModelIndex &start, int role, const QVariant &value, int hits,
                                      Qt::MatchFlags flags) const {
    return QAbstractListModel::match(start, role, value, hits, flags);
}

int ProgrammeModel::match(int start, const QByteArray &role, const QVariant &value, int flags) const {
    const QModelIndexList idxs = match(index(start), m_roles.key(role), value, 1, Qt::MatchFlags(flags));
    return idxs.isEmpty() ? -1 : idxs.first().row();
}

void ProgrammeModel::clear() {
    if ((m_programmes) && (!m_programmes->isEmpty())) {
	beginResetModel();
	m_programmes->clear();
	endResetModel();
    }
}

void ProgrammeModel::reload(const QString &ref) {
    if (m_programmes) {
        beginResetModel();
        m_programmes->reload(ref);
        endResetModel();
    }
}

void ProgrammeModel::cancel() {
    if (m_programmes) {
        m_programmes->cancel();
    }
}

void ProgrammeModel::onFinished() {
    if ((m_programmes) && (m_programmes->status() == Request::Ready) && (!m_programmes->isEmpty())) {
        beginResetModel();
        endResetModel();
    }    
}
