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

#ifndef ENIGMA1SCREENSHOT_H
#define ENIGMA1SCREENSHOT_H

#include "screenshot.h"

class Enigma1ScreenShot : public ScreenShot
{
    Q_OBJECT

public:
    explicit Enigma1ScreenShot(QObject *parent = 0);

    QString address() const;
    void setAddress(const QString &a);

    QString username() const;
    void setUsername(const QString &u);

    QString password() const;
    void setPassword(const QString &p);

    QString errorString() const;

    Request::Status status() const;

    QImage image() const;
    QPixmap pixmap() const;

public Q_SLOTS:    
    void reload(int imageType);

    bool save(const QString &fileName);

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
    
    int m_imageType;
    
    bool m_imageLoaded;
};

#endif // ENIGMA1SCREENSHOT_H
