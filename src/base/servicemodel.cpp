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

#include "servicemodel.h"
#include "service.h"
#include <QFont>

ServiceModel::ServiceModel(QObject *parent) :
    QStandardItemModel(parent),
    m_services(0)
{
    m_roles[Qt::DisplayRole] = "display";
    m_roles[IdRole] = "id";
    m_roles[TitleRole] = "title";
    m_roles[ServiceTypeRole] = "serviceType";
    m_roles[SectionRole] = "section";
#if QT_VERSION < 0x050000
    setRoleNames(m_roles);
#endif
}

ServiceList* ServiceModel::services() const {
    return m_services;
}

void ServiceModel::setServices(ServiceList *s) {
    if (s == m_services) {
        return;
    }
        
    if (m_services) {
        disconnect(m_services, SIGNAL(finished()), this, SLOT(onFinished()));
        disconnect(m_services, SIGNAL(statusChanged(Request::Status)),
                   this, SIGNAL(statusChanged(Request::Status)));
    }

    m_services = s;

    if (m_services) {
        connect(m_services, SIGNAL(finished()), this, SLOT(onFinished()));
        connect(m_services, SIGNAL(statusChanged(Request::Status)),
                this, SIGNAL(statusChanged(Request::Status)));
    }

    clear();
    emit servicesChanged();
}

Request::Status ServiceModel::status() const {
    return m_services ? m_services->status() : Request::Null;
}

QString ServiceModel::errorString() const {
    return m_services ? m_services->errorString() : QString();
}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> ServiceModel::roleNames() const {
    return m_roles;
}
#endif

QVariant ServiceModel::data(const QModelIndex &index, int role) const {
    return QStandardItemModel::data(index, role);
}

QVariant ServiceModel::data(int row, const QVariant &parentIndex, const QByteArray &role) const {
    return data(index(row, 0, qvariant_cast<QModelIndex>(parentIndex)), m_roles.key(role));
}

bool ServiceModel::setData(const QModelIndex &index, const QVariant &data, int role) {
    return QStandardItemModel::setData(index, data, role);
}

bool ServiceModel::setData(int row, const QVariant &parentIndex, const QVariant &data,
                           const QByteArray &role) {
    return setData(index(row, 0, qvariant_cast<QModelIndex>(parentIndex)), data,
                   m_roles.key(role));
}

QVariant ServiceModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((section == 0) && (orientation == Qt::Horizontal)) {
        switch (role) {
        case Qt::DisplayRole:
            return tr("Services");
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

QMap<int, QVariant> ServiceModel::itemData(const QModelIndex &index) const {
    return QStandardItemModel::itemData(index);
}

QVariantMap ServiceModel::itemData(int row, const QVariant &parentIndex) const {
    QVariantMap map;

    foreach (const QByteArray &role, m_roles.values()) {
        map[role] = data(row, parentIndex, role);
    }

    return map;
}

bool ServiceModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &data) {
    return QStandardItemModel::setItemData(index, data);
}

bool ServiceModel::setItemData(int row, const QVariant &parentIndex, const QVariantMap &data) {
    if (data.isEmpty()) {
        return false;
    }

    QMapIterator<QString, QVariant> iterator(data);

    while (iterator.hasNext()) {
        iterator.next();

        if (!setData(row, parentIndex, iterator.value(), iterator.key().toUtf8())) {
            return false;
        }
    }

    return true;
}

QModelIndexList ServiceModel::match(const QModelIndex &start, int role, const QVariant &value, int hits,
                                    Qt::MatchFlags flags) const {
    return QStandardItemModel::match(start, role, value, hits, flags);
}

int ServiceModel::match(int start, const QVariant &parentIndex, const QByteArray &role, const QVariant &value,
                        int flags) const {
    const QModelIndexList idxs = match(index(start, 0, qvariant_cast<QModelIndex>(parentIndex)), m_roles.key(role),
                                       value, Qt::MatchFlags(flags));
    return idxs.isEmpty() ? -1 : idxs.first().row();
}

void ServiceModel::reload(const QModelIndex &parent) {
    if ((m_services) && (m_services->status() != Request::Active)) {
        removeRows(0, rowCount(parent), parent);
        m_parent = QPersistentModelIndex(parent);
        m_services->reload(parent.data(IdRole).toString());
    }
}

void ServiceModel::reload(const QVariant &parentIndex) {
    reload(qvariant_cast<QModelIndex>(parentIndex));
}

void ServiceModel::cancel() {
    if (m_services) {
        m_services->cancel();
    }
}

void ServiceModel::onFinished() {
    if ((m_services) && (m_services->status() == Request::Ready) && (!m_services->isEmpty())) {
        QStandardItem *parent = itemFromIndex(m_parent);

        if (!parent) {
            parent = invisibleRootItem();
        }

        if (parent) {
            for (int i = 0; i < m_services->count(); i++) {
                QStandardItem *item = new QStandardItem(m_services->at(i)->title());
                item->setData(m_services->at(i)->id(), IdRole);
                item->setData(m_services->at(i)->title(), TitleRole);
                item->setData(m_services->at(i)->serviceType(), ServiceTypeRole);
                item->setData(m_services->at(i)->title().left(1).toUpper(), SectionRole);
                parent->appendRow(item);
            }
        }

        m_services->clear();
    }
}
