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

#ifndef PROGRAMMETIMERLIST_H
#define PROGRAMMETIMERLIST_H

#include "request.h"

class ProgrammeTimer;

class ProgrammeTimerList : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(bool empty READ isEmpty NOTIFY countChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString NOTIFY finished FINAL)
    Q_PROPERTY(Request::Status status READ status NOTIFY statusChanged FINAL)

    Q_ENUMS(Request::Status)

public:
    explicit ProgrammeTimerList(QObject *parent = 0) :
        QObject(parent)
    {
    }

    virtual QString address() const = 0;
    virtual void setAddress(const QString &) = 0;

    virtual QString username() const { return QString(); }
    virtual void setUsername(const QString &) {}

    virtual QString password() const { return QString(); }
    virtual void setPassword(const QString &) {}
    
    virtual int count() const = 0;
    bool isEmpty() const { return count() == 0; }

    virtual QString errorString() const { return QString(); }

    virtual Request::Status status() const = 0;
    
    Q_INVOKABLE virtual ProgrammeTimer* at(int i) const = 0;

public Q_SLOTS:
    virtual void clear() = 0;
    virtual void reload() = 0;
    virtual void cancel() = 0;

Q_SIGNALS:
    void addressChanged();
    void usernameChanged();
    void passwordChanged();
    void countChanged(int c);
    void statusChanged(Request::Status s);
    void finished();
};

#endif // PROGRAMMETIMERLIST_H
