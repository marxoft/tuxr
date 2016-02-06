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

#include "remoteconfigs.h"
#include <QSettings>
#include <QStringList>

static QVariant value(const QString &id, const QString &property) {
    return QSettings("TuxR", "Remotes").value(QString("%1/%2").arg(id).arg(property));
}

static void setValue(const QString &id, const QString &property, const QVariant &value) {
    QSettings("TuxR", "Remotes").setValue(QString("%1/%2").arg(id).arg(property), value);
}

RemoteConfigs* RemoteConfigs::self = 0;

RemoteConfigs::RemoteConfigs() :
    QObject()
{
}

RemoteConfigs::~RemoteConfigs() {
    self = 0;
}

RemoteConfigs* RemoteConfigs::instance() {
    return self ? self : self = new RemoteConfigs;
}

void RemoteConfigs::add(int remoteType, const QString &name, const QString &address, int streamingPort,
                        const QString &username, const QString &password) {
    QSettings settings("TuxR", "Remotes");
    const QStringList groups = settings.childGroups();
    const QString id = QString("%1").arg(groups.isEmpty() ? 1 : qMax(1, groups.last().toInt() + 1),
                                         2, 10, QLatin1Char('0'));
    
    settings.beginGroup(id);
    settings.setValue("remoteType", remoteType);
    settings.setValue("name", name);
    settings.setValue("address", address);
    settings.setValue("streamingPort", streamingPort);
    settings.setValue("username", username);
    settings.setValue("password", password);
    settings.endGroup();
    
    if (self) {
        emit self->added(id);
    }
}

bool RemoteConfigs::remove(const QString &id) {
    QSettings settings("TuxR", "Remotes");
    
    if (settings.childGroups().contains(id)) {
        settings.remove(id);
        
        if (self) {
            emit self->removed(id);
        }
        
        return true;
    }
    
    return false;
}

int RemoteConfigs::remoteType(const QString &id) {
    return value(id, "remoteType").toInt();
}

void RemoteConfigs::setRemoteType(const QString &id, int t) {
    if (t != remoteType(id)) {
        setValue(id, "remoteType", t);
        
        if (self) {
            emit self->changed(id);
        }
    }
}

QString RemoteConfigs::name(const QString &id) {
    return value(id, "name").toString();
}

void RemoteConfigs::setName(const QString &id, const QString &n) {
    if (n != name(id)) {
        setValue(id, "name", n);
        
        if (self) {
            emit self->changed(id);
        }
    }
}

QString RemoteConfigs::address(const QString &id) {
    return value(id, "address").toString();
}

void RemoteConfigs::setAddress(const QString &id, const QString &a) {
    if (a != address(id)) {
        setValue(id, "address", a);
        
        if (self) {
            emit self->changed(id);
        }
    }
}

int RemoteConfigs::streamingPort(const QString &id) {
    return value(id, "streamingPort").toInt();
}

void RemoteConfigs::setStreamingPort(const QString &id, int p) {
    if (p != streamingPort(id)) {
        setValue(id, "streamingPort", p);
        
        if (self) {
            emit self->changed(id);
        }
    }
}

QString RemoteConfigs::username(const QString &id) {
    return value(id, "username").toString();
}

void RemoteConfigs::setUsername(const QString &id, const QString &u) {
    if (u != username(id)) {
        setValue(id, "username", u);
        
        if (self) {
            emit self->changed(id);
        }
    }
}

QString RemoteConfigs::password(const QString &id) {
    return value(id, "password").toString();
}

void RemoteConfigs::setPassword(const QString &id, const QString &p) {
    if (p != password(id)) {
        setValue(id, "password", p);
        
        if (self) {
            emit self->changed(id);
        }
    }
}

QVariantList RemoteConfigs::macros(const QString &id) {
    QSettings settings("TuxR", "Remotes");
    settings.beginGroup(id);
    const int size = settings.beginReadArray("macros");
    QVariantList macros;
    
    for (int i = 0; i < size; i++) {
        QVariantMap macro;
        macro["text"] = settings.value("text").toString();
        macro["keys"] = settings.value("keys");
        macros << macro;
    }
    
    settings.endArray();
    settings.endGroup();
    
    return macros;
}

void RemoteConfigs::setMacros(const QString &id, const QVariantList &m) {
    QSettings settings("TuxR", "Remotes");
    settings.beginGroup(id);
    settings.beginWriteArray("macros");
    
    for (int i = 0; i < m.size(); i++) {
        const QVariantMap macro = m.at(i).toMap();
        settings.setArrayIndex(i);
        settings.setValue("text", macro.value("text"));
        settings.setValue("keys", macro.value("keys"));
    }
    
    settings.endArray();
    settings.endGroup();
    
    if (self) {
        emit self->changed(id);
    }
}
