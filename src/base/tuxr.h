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

#ifndef TUXR_H
#define TUXR_H

#include <QObject>

class TuxR : public QObject
{
    Q_OBJECT
    
    Q_ENUMS(RemoteType)

public:
    enum RemoteType {
        Enigma1 = 0,
        Unknown
    };

private:
    TuxR() : QObject() {}
};

#endif // TUXR_H
