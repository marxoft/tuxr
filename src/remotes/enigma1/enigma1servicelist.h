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

#ifndef ENIGMA1SERVICELIST_H
#define ENIGMA1SERVICELIST_H

#include "servicelist.h"

class Enigma1ServiceList : public ServiceList
{
    Q_OBJECT
    
public:
    explicit Enigma1ServiceList(QObject *parent = 0);

    QString address() const;
    void setAddress(const QString &a);

    QString username() const;
    void setUsername(const QString &u);

    QString password() const;
    void setPassword(const QString &p);

    QString errorString() const;
    
    Request::Status status() const;

    int count() const;

    Q_INVOKABLE Service* at(int i) const;

public Q_SLOTS:
    void clear();
    void reload(const QString &ref);
    void cancel();

protected:
    void setErrorString(const QString &e);

    void setStatus(Request::Status s);

protected Q_SLOTS:
    void onRequestFinished();

protected:
    Request *m_request;

    QString m_address;
    QString m_username;
    QString m_password;

    QString m_errorString;

    Request::Status m_status;

    QList<Service*> m_services;
};

#endif // ENIGMA1SERVICELIST_H
