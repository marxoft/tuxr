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

#include "settings.h"
#include <QSettings>

static QVariant value(const QString &property, const QVariant &defaultValue = QVariant()) {
    return QSettings().value(property, defaultValue);
}

static void setValue(const QString &property, const QVariant &value) {
    QSettings().setValue(property, value);
}

Settings* Settings::self = 0;

Settings::Settings() :
    QObject()
{
}

Settings::~Settings() {
    self = 0;
}

Settings* Settings::instance() {
    return self ? self : self = new Settings;
}

QString Settings::currentRemote() {
    return value("currentRemote").toString();
}

void Settings::setCurrentRemote(const QString &id) {
    if (id != currentRemote()) {
        setValue("currentRemote", id);
        
        if (self) {
            emit self->currentRemoteChanged();
        }
    }
}

int Settings::screenOrientation() {
    return value("screenOrientation", 0).toInt();
}

void Settings::setScreenOrientation(int orientation) {
    if (orientation != screenOrientation()) {
        setValue("screenOrientation", orientation);
        
        if (self) {
            emit self->screenOrientationChanged();
        }
    }
}

int Settings::screenShotImageType() {
    return value("screenShotImageType", 0).toInt();
}

void Settings::setScreenShotImageType(int imageType) {
    if (imageType != screenShotImageType()) {
        setValue("screenShotImageType", imageType);
        
        if (self) {
            emit self->screenShotImageTypeChanged();
        }
    }
}
