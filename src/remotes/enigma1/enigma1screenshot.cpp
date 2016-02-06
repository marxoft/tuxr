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

#include "enigma1screenshot.h"
#include <QDir>

Enigma1ScreenShot::Enigma1ScreenShot(QObject *parent) :
    ScreenShot(parent),
    m_request(0),
    m_status(Request::Null),
    m_imageType(ScreenOnly),
    m_imageLoaded(false)
{
}

QString Enigma1ScreenShot::address() const {
    return m_address;
}

void Enigma1ScreenShot::setAddress(const QString &a) {
    if (a != address()) {
        m_address = a;
        emit addressChanged();
    }
}

QString Enigma1ScreenShot::username() const {
    return m_username;
}

void Enigma1ScreenShot::setUsername(const QString &u) {
    if (u != username()) {
        m_username = u;
        emit usernameChanged();
    }
}

QString Enigma1ScreenShot::password() const {
    return m_password;
}

void Enigma1ScreenShot::setPassword(const QString &p) {
    if (p != password()) {
        m_password = p;
        emit passwordChanged();
    }
}

QString Enigma1ScreenShot::errorString() const {
    return m_errorString;
}

void Enigma1ScreenShot::setErrorString(const QString &e) {
    m_errorString = e;
}

Request::Status Enigma1ScreenShot::status() const {
    return m_status;
}

void Enigma1ScreenShot::setStatus(Request::Status s) {
    if (s != status()) {
        m_status = s;
        emit statusChanged(s);
    }
}

QImage Enigma1ScreenShot::image() const {
    return status() == Request::Ready ? QImage::fromData(m_request->result()) : QImage();
}

QPixmap Enigma1ScreenShot::pixmap() const {
    QPixmap p;

    if (status() == Request::Ready) {
        p.loadFromData(m_request->result());
    }

    return p;
}

void Enigma1ScreenShot::reload(int imageType) {
    if (status() == Request::Active) {
        return;
    }

    m_imageType = imageType;
    m_imageLoaded = false;

    switch (imageType) {
    case OsdOnly:
        send("/body?mode=controlFBShot");
        break;
    case ScreenAndOsd:
        send("/body?mode=controlScreenShot&blendtype=2");
        break;
    default:
        send("/body?mode=controlScreenShot");
        break;
    }
}

bool Enigma1ScreenShot::save(const QString &fileName) {
    return (QDir().mkpath(fileName.left(fileName.lastIndexOf("/")))) && (image().save(fileName));
}

void Enigma1ScreenShot::send(const QString &command) {
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

void Enigma1ScreenShot::cancel() {
    if (m_request) {
        m_request->cancel();
    }
}

void Enigma1ScreenShot::onRequestFinished() {
    if (!m_request) {
        return;
    }

    if (m_request->status() == Request::Ready) {
        if (!m_imageLoaded) {
            m_imageLoaded = true;
            
            if (m_imageType == OsdOnly) {
                send("/root/tmp/osdshot.png");
            }
            else {
                send("/root/tmp/screenshot.bmp");
            }
            
            return;
        }
    }

    setErrorString(m_request->errorString());
    setStatus(m_request->status());
    emit finished();
}
