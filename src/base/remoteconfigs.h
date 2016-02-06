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

#ifndef REMOTECONFIGS_H
#define REMOTECONFIGS_H

#include <QObject>
#include <QVariantList>

class RemoteConfigs : public QObject
{
    Q_OBJECT
    
public:
    ~RemoteConfigs();
    
    static RemoteConfigs* instance();

public Q_SLOTS:
    static void add(int remoteType, const QString &name, const QString &address, int streamingPort,
                    const QString &username, const QString &password);
    
    static bool remove(const QString &id);
    
    static int remoteType(const QString &id);
    static void setRemoteType(const QString &id, int t);
    
    static QString name(const QString &id);
    static void setName(const QString &id, const QString &n);
    
    static QString address(const QString &id);
    static void setAddress(const QString &id, const QString &a);
    
    static int streamingPort(const QString &id);
    static void setStreamingPort(const QString &id, int p);
    
    static QString username(const QString &id);
    static void setUsername(const QString &id, const QString &u);
    
    static QString password(const QString &id);
    static void setPassword(const QString &id, const QString &u);
    
    static QVariantList macros(const QString &id);
    static void setMacros(const QString &id, const QVariantList &m);

Q_SIGNALS:
    void added(const QString &id);
    void changed(const QString &id);
    void removed(const QString &id);

private:
    RemoteConfigs();
    
    static RemoteConfigs *self;
};

#endif // REMOTECONFIGS_H
