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

#include "enigma1remote.h"

Enigma1Remote::Enigma1Remote(QObject *parent) :
    Remote(parent),
    m_request(0),
    m_status(Request::Null)
{
}

QString Enigma1Remote::address() const {
    return m_address;
}

void Enigma1Remote::setAddress(const QString &a) {
    if (a != address()) {
        m_address = a;
        emit addressChanged();
    }
}

QString Enigma1Remote::username() const {
    return m_username;
}

void Enigma1Remote::setUsername(const QString &u) {
    if (u != username()) {
        m_username = u;
        emit usernameChanged();
    }
}

QString Enigma1Remote::password() const {
    return m_password;
}

void Enigma1Remote::setPassword(const QString &p) {
    if (p != password()) {
        m_password = p;
        emit passwordChanged();
    }
}

QString Enigma1Remote::errorString() const {
    return m_errorString;
}

void Enigma1Remote::setErrorString(const QString &e) {
    m_errorString = e;
}

Request::Status Enigma1Remote::status() const {
    return m_status;
}

void Enigma1Remote::setStatus(Request::Status s) {
    if (s != status()) {
        m_status = s;
        emit statusChanged(s);
    }
}

void Enigma1Remote::sendCommand(const QString &command) {
    m_commands << command;

    if (status() != Request::Active) {
        send(m_commands.takeFirst());
    }
}

void Enigma1Remote::sendCommands(const QStringList &commands) {
    if (commands.isEmpty()) {
        return;
    }
    
    m_commands << commands;

    if (status() != Request::Active) {
        send(m_commands.takeFirst());
    }
}

void Enigma1Remote::sendKey(int key) {
    switch (key) {
    case Key_0:
        sendCommand("/cgi-bin/rc?11");
        break;
    case Key_1:
        sendCommand("/cgi-bin/rc?2");
        break;
    case Key_2:
        sendCommand("/cgi-bin/rc?3");
        break;
    case Key_3:
        sendCommand("/cgi-bin/rc?4");
        break;
    case Key_4:
        sendCommand("/cgi-bin/rc?5");
        break;
    case Key_5:
        sendCommand("/cgi-bin/rc?6");
        break;
    case Key_6:
        sendCommand("/cgi-bin/rc?7");
        break;
    case Key_7:
        sendCommand("/cgi-bin/rc?8");
        break;
    case Key_8:
        sendCommand("/cgi-bin/rc?9");
        break;
    case Key_9:
        sendCommand("/cgi-bin/rc?10");
        break;
    case Key_Back:
        sendCommand("/cgi-bin/rc?412");
        break;
    case Key_Forward:
        sendCommand("/cgi-bin/rc?407");
        break;
    case Key_Up:
        sendCommand("/cgi-bin/rc?103");
        break;
    case Key_Down:
        sendCommand("/cgi-bin/rc?108");
        break;
    case Key_Left:
        sendCommand("/cgi-bin/rc?105");
        break;
    case Key_Right:
        sendCommand("/cgi-bin/rc?106");
        break;
    case Key_Enter:
        sendCommand("/cgi-bin/rc?352");
        break;
    case Key_VolumeUp:
        sendCommand("/cgi-bin/rc?115");
        break;
    case Key_VolumeDown:
        sendCommand("/cgi-bin/rc?114");
        break;
    case Key_VolumeMute:
        sendCommand("/cgi-bin/rc?113");
        break;
    case Key_BouquetUp:
        sendCommand("/cgi-bin/rc?402");
        break;
    case Key_BouquetDown:
        sendCommand("/cgi-bin/rc?403");
        break;
    case Key_Info:
        sendCommand("/cgi-bin/rc?358");
        break;
    case Key_Menu:
        sendCommand("/cgi-bin/rc?141");
        break;
    case Key_Exit:
        sendCommand("/cgi-bin/rc?1");
        break;
    case Key_Audio:
        sendCommand("/cgi-bin/rc?392");
        break;
    case Key_Video:
        sendCommand("/cgi-bin/rc?393");
        break;
    case Key_Red:
        sendCommand("/cgi-bin/rc?398");
        break;
    case Key_Green:
        sendCommand("/cgi-bin/rc?399");
        break;
    case Key_Yellow:
        sendCommand("/cgi-bin/rc?400");
        break;
    case Key_Blue:
        sendCommand("/cgi-bin/rc?401");
        break;
    case Key_Power:
        sendCommand("/cgi-bin/rc?116");
        break;
    default:
        m_commands.clear();
        setErrorString(tr("Invalid remote key specified."));
        setStatus(Request::Error);
        emit finished();
        break;
    }
}

void Enigma1Remote::sendKeys(const QVariantList &keys) {
    foreach (const QVariant &key, keys) {
        sendKey(key.toInt());
    }
}

void Enigma1Remote::changeChannel(const QString &ref) {
    sendCommand("/cgi-bin/zapTo?path=" + ref);
}

void Enigma1Remote::send(const QString &command) {
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

void Enigma1Remote::cancel() {
    if (m_request) {
        m_request->cancel();
    }

    m_commands.clear();
}

void Enigma1Remote::onRequestFinished() {
    if (!m_request) {
        return;
    }

    if ((m_request->status() == Request::Ready) && (!m_commands.isEmpty())) {
        send(m_commands.takeFirst());
        return;
    }

    m_commands.clear();
    setErrorString(m_request->errorString());
    setStatus(m_request->status());
    emit finished();
}
