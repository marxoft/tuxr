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

#include "service.h"

Service::Service(QObject *parent) :
    QObject(parent),
    m_serviceType(Unknown)
{
}

Service::Service(const QString &id, const QString &title, ServiceType serviceType,
                 QObject *parent) :
    QObject(parent),
    m_id(id),
    m_title(title),
    m_serviceType(serviceType)
{
}

QString Service::id() const {
    return m_id;
}

void Service::setId(const QString &i) {
    if (i != id()) {
        m_id = i;
        emit idChanged();
    }
}

QString Service::title() const {
    return m_title;
}

void Service::setTitle(const QString &t) {
    if (t != title()) {
        m_title = t;
        emit titleChanged();
    }
}

Service::ServiceType Service::serviceType() const {
    return m_serviceType;
}

void Service::setServiceType(Service::ServiceType t) {
    if (t != serviceType()) {
        m_serviceType = t;
        emit serviceTypeChanged();
    }
}
