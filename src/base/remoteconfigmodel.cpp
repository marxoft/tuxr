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

#include "remoteconfigmodel.h"
#include "remoteconfigs.h"
#include <QSettings>

RemoteConfigModel::RemoteConfigModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_roles[Qt::DisplayRole] = "display";
    m_roles[IdRole] = "id";
    m_roles[RemoteTypeRole] = "remoteType";
    m_roles[NameRole] = "name";
    m_roles[AddressRole] = "address";
    m_roles[StreamingPortRole] = "streamingPort";
    m_roles[UsernameRole] = "username";
    m_roles[PasswordRole] = "password";
    m_roles[MacrosRole] = "macros";
#if QT_VERSION < 0x050000
    setRoleNames(m_roles);
#endif
    connect(RemoteConfigs::instance(), SIGNAL(added(QString)), this, SLOT(onRemoteAdded(QString)));
    connect(RemoteConfigs::instance(), SIGNAL(changed(QString)), this, SLOT(onRemoteChanged(QString)));
    connect(RemoteConfigs::instance(), SIGNAL(removed(QString)), this, SLOT(onRemoteRemoved(QString)));
}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> RemoteConfigModel::roleNames() const {
    return m_roles;
}
#endif

int RemoteConfigModel::rowCount(const QModelIndex &) const {
    return m_remoteIds.size();
}

QVariant RemoteConfigModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
    case NameRole:
        return RemoteConfigs::name(m_remoteIds.at(index.row()));
    case IdRole:
        return m_remoteIds.at(index.row());
    case RemoteTypeRole:
        return RemoteConfigs::remoteType(m_remoteIds.at(index.row()));
    case AddressRole:
        return RemoteConfigs::address(m_remoteIds.at(index.row()));
    case StreamingPortRole:
        return RemoteConfigs::streamingPort(m_remoteIds.at(index.row()));
    case UsernameRole:
        return RemoteConfigs::username(m_remoteIds.at(index.row()));
    case PasswordRole:
        return RemoteConfigs::password(m_remoteIds.at(index.row()));
    case MacrosRole:
        return RemoteConfigs::macros(m_remoteIds.at(index.row()));
    default:
        return QVariant();
    }
}

QVariant RemoteConfigModel::data(int row, const QByteArray &role) const {
    return data(index(row), m_roles.key(role));
}

bool RemoteConfigModel::setData(const QModelIndex &index, const QVariant &data, int role) {
    if (!index.isValid()) {
        return false;
    }

    switch (role) {
    case Qt::DisplayRole:
    case NameRole:
        RemoteConfigs::setName(m_remoteIds.at(index.row()), data.toString());
        break;
    case RemoteTypeRole:
        RemoteConfigs::setRemoteType(m_remoteIds.at(index.row()), data.toInt());
        break;
    case AddressRole:
        RemoteConfigs::setAddress(m_remoteIds.at(index.row()), data.toString());
        break;
    case StreamingPortRole:
        RemoteConfigs::setStreamingPort(m_remoteIds.at(index.row()), data.toInt());
        break;
    case UsernameRole:
        RemoteConfigs::setUsername(m_remoteIds.at(index.row()), data.toString());
        break;
    case PasswordRole:
        RemoteConfigs::setPassword(m_remoteIds.at(index.row()), data.toString());
        break;
    case MacrosRole:
        RemoteConfigs::setMacros(m_remoteIds.at(index.row()), data.toList());
        break;
    default:
        return false;
    }

    return true;
}

bool RemoteConfigModel::setData(int row, const QVariant &data, const QByteArray &role) {
    return setData(index(row), data, m_roles.key(role));
}

QVariant RemoteConfigModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((section == 0) && (orientation == Qt::Vertical) && (role == Qt::DisplayRole)) {
        return tr("Remotes");
    }

    return QVariant();
}

QMap<int, QVariant> RemoteConfigModel::itemData(const QModelIndex &index) const {
    QMap<int, QVariant> map;

    if (!index.isValid()) {
        return map;
    }
    
    foreach (int role, m_roles.keys()) {
        map[role] = data(index, role);
    }

    return map;
}

QVariantMap RemoteConfigModel::itemData(int row) const {
    QVariantMap map;

    if ((row < 0) || (row >= m_remoteIds.size())) {
        return map;
    }

    foreach (const QByteArray &role, m_roles.values()) {
        map[QString::fromUtf8(role)] = data(row, role);
    }
    
    return map;
}

bool RemoteConfigModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &data) {
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

bool RemoteConfigModel::setItemData(int row, const QVariantMap &data) {
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

QModelIndexList RemoteConfigModel::match(const QModelIndex &start, int role, const QVariant &value, int hits,
                                         Qt::MatchFlags flags) const {
    return QAbstractListModel::match(start, role, value, hits, flags);
}

int RemoteConfigModel::match(int start, const QByteArray &role, const QVariant &value, int flags) const {
    const QModelIndexList idxs = match(index(start), m_roles.key(role), value, 1, Qt::MatchFlags(flags));
    return idxs.isEmpty() ? -1 : idxs.first().row();
}

void RemoteConfigModel::reload() {
    clear();
    beginResetModel();
    m_remoteIds = QSettings("TuxR", "Remotes").childGroups();
    endResetModel();
    emit countChanged(rowCount());
}

void RemoteConfigModel::clear() {
    if (!m_remoteIds.isEmpty()) {
        beginResetModel();
        m_remoteIds.clear();
        endResetModel();
        emit countChanged(0);
    }
}

void RemoteConfigModel::onRemoteAdded(const QString &id) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_remoteIds << id;
    endInsertRows();
    emit countChanged(rowCount());
}

void RemoteConfigModel::onRemoteChanged(const QString &id) {
    for (int i = 0; i < m_remoteIds.size(); i++) {
        if (m_remoteIds.at(i) == id) {
            const QModelIndex idx = index(i);
            emit dataChanged(idx, idx);
        }
    }
}

void RemoteConfigModel::onRemoteRemoved(const QString &id) {
    for (int i = 0; i < m_remoteIds.size(); i++) {
        if (m_remoteIds.at(i) == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_remoteIds.removeAt(i);
            endRemoveRows();
            emit countChanged(rowCount());
            return;
        }
    }
}
