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

#ifndef REMOTE_H
#define REMOTE_H

#include "request.h"

class Remote : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString NOTIFY finished FINAL)
    Q_PROPERTY(Request::Status status READ status NOTIFY statusChanged FINAL)

    Q_ENUMS(Key Request::Status)

public:
    enum Key {
        Key_0 = 0,
        Key_1,
        Key_2,
        Key_3,
        Key_4,
        Key_5,
        Key_6,
        Key_7,
        Key_8,
        Key_9,
        Key_Back,
        Key_Forward,
        Key_Up,
        Key_Down,
        Key_Left,
        Key_Right,
        Key_Enter,
        Key_VolumeUp,
        Key_VolumeDown,
        Key_VolumeMute,
        Key_BouquetUp,
        Key_BouquetDown,
        Key_Info,
        Key_Menu,
        Key_Exit,
        Key_Audio,
        Key_Video,
        Key_Red,
        Key_Green,
        Key_Yellow,
        Key_Blue,
        Key_Tv,
        Key_Radio,
        Key_Text,
        Key_Help,
        Key_Power
    };
    
    explicit Remote(QObject *parent = 0) :
        QObject(parent)
    {
    }
    
    virtual QString address() const = 0;
    virtual void setAddress(const QString &) = 0;

    virtual QString username() const { return QString(); }
    virtual void setUsername(const QString &) {}

    virtual QString password() const { return QString(); }
    virtual void setPassword(const QString &) {}

    virtual QString errorString() const { return QString(); }

    virtual Request::Status status() const = 0;

    Q_INVOKABLE virtual bool keyIsEnabled(int key) { return (key >= Key_0) && (key <= Key_Power); }

public Q_SLOTS:
    virtual void sendCommand(const QString &) = 0;
    virtual void sendCommands(const QStringList &) = 0;

    virtual void sendKey(int key) = 0;
    virtual void sendKeys(const QVariantList &) = 0;
    
    virtual void changeChannel(const QString &) = 0;
    
    virtual void cancel() = 0;

Q_SIGNALS:
    void addressChanged();
    void usernameChanged();
    void passwordChanged();
    void statusChanged(Request::Status s);
    void finished();
};

#endif // REMOTE_H
