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

#ifndef ENIGMA1REMOTE_H
#define ENIGMA1REMOTE_H

#include "remote.h"
#include <QStringList>

class Enigma1Remote : public Remote
{
    Q_OBJECT

public:
    explicit Enigma1Remote(QObject *parent = 0);

    QString address() const;
    void setAddress(const QString &a);

    QString username() const;
    void setUsername(const QString &u);

    QString password() const;
    void setPassword(const QString &p);

    QString errorString() const;

    Request::Status status() const;

public Q_SLOTS:    
    void sendCommand(const QString &command);
    void sendCommands(const QStringList &commands);

    void sendKey(int key);
    void sendKeys(const QVariantList &keys);

    void changeChannel(const QString &ref);

    void cancel();

private:    
    void setErrorString(const QString &e);

    void setStatus(Request::Status s);

    void send(const QString &command);

private Q_SLOTS:
    void onRequestFinished();

private:
    Request *m_request;
    
    QString m_address;
    QString m_username;
    QString m_password;

    QString m_errorString;

    Request::Status m_status;
    
    QStringList m_commands;
};

#endif // ENIGMA1REMOTE_H
