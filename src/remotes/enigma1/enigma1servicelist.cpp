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

#include "enigma1servicelist.h"
#include "service.h"
#include <QStringList>

bool serviceTitleLessThan(Service *s1, Service *s2) {
    return s1->title().toLower() < s2->title().toLower();
}

Enigma1ServiceList::Enigma1ServiceList(QObject *parent) :
    ServiceList(parent),
    m_request(0),
    m_status(Request::Null)
{
}

QString Enigma1ServiceList::address() const {
    return m_address;
}

void Enigma1ServiceList::setAddress(const QString &a) {
    if (a != address()) {
        m_address = a;
        emit addressChanged();
    }
}

QString Enigma1ServiceList::username() const {
    return m_username;
}

void Enigma1ServiceList::setUsername(const QString &u) {
    if (u != username()) {
        m_username = u;
        emit usernameChanged();
    }
}

QString Enigma1ServiceList::password() const {
    return m_password;
}

void Enigma1ServiceList::setPassword(const QString &p) {
    if (p != password()) {
        m_password = p;
        emit passwordChanged();
    }
}

QString Enigma1ServiceList::errorString() const {
    return m_errorString;
}

void Enigma1ServiceList::setErrorString(const QString &e) {
    m_errorString = e;
}

Request::Status Enigma1ServiceList::status() const {
    return m_status;
}

void Enigma1ServiceList::setStatus(Request::Status s) {
    if (s != status()) {
        m_status = s;
        emit statusChanged(s);
    }
}

int Enigma1ServiceList::count() const {
    return m_services.size();
}

Service* Enigma1ServiceList::at(int i) const {
    if ((i >= 0) && (i < m_services.size())) {
        return m_services.at(i);
    }

    return 0;
}

void Enigma1ServiceList::clear() {
    if (!m_services.isEmpty()) {
        qDeleteAll(m_services);
        m_services.clear();
        emit countChanged(0);
    }
}

void Enigma1ServiceList::reload(const QString &ref) {
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

    m_request->get(QUrl::fromUserInput(QString(address() + "/cgi-bin/getServices?ref="
                   + (ref.isEmpty() ? QString("0") : ref))));
}

void Enigma1ServiceList::cancel() {
    if (m_request) {
        m_request->cancel();
    }
}

void Enigma1ServiceList::onRequestFinished() {
    if (!m_request) {
        return;
    }

    if (m_request->status() == Request::Ready) {
        const QStringList services =
        QString::fromUtf8(m_request->result()).split("\n", QString::SkipEmptyParts);
        
        foreach (const QString &service, services) {
            int i = service.indexOf(";");
            const QString id = service.left(i);
            const QString title = service.mid(i + 1).section(";", 0, 0);
            m_services << new Service(id, title, id.section("/", 0, 0).endsWith(":0:0:0:0:0:")
                                      ? Service::Bouquet : Service::Channel, this);
        }

        qSort(m_services.begin(), m_services.end(), serviceTitleLessThan);
        emit countChanged(count());
    }

    setErrorString(m_request->errorString());
    setStatus(m_request->status());
    emit finished();
}
