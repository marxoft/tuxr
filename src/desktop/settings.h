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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString currentRemote READ currentRemote WRITE setCurrentRemote
               NOTIFY currentRemoteChanged)

public:
    ~Settings();

    static Settings* instance();

    static QString currentRemote();

public Q_SLOTS:
    static void setCurrentRemote(const QString &id);

Q_SIGNALS:
    void currentRemoteChanged(const QString &id);

private:
    Settings();

    static Settings *self;
};

#endif // SETTINGS_H
