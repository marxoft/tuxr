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

#ifndef REMOTEKEYS_H
#define REMOTEKEYS_H

#include "remote.h"

class RemoteKeys : public QObject
{
    Q_OBJECT

public:
    explicit RemoteKeys(QObject *parent = 0)
        : QObject(parent)
    {
    }
    
public Q_SLOTS:
    static QString text(int key) {
        switch (key) {
        case Remote::Key_0:
            return QString("0");
        case Remote::Key_1:
            return QString("1");
        case Remote::Key_2:
            return QString("2");
        case Remote::Key_3:
            return QString("3");
        case Remote::Key_4:
            return QString("4");
        case Remote::Key_5:
            return QString("5");
        case Remote::Key_6:
            return QString("6");
        case Remote::Key_7:
            return QString("7");
        case Remote::Key_8:
            return QString("8");
        case Remote::Key_9:
            return QString("9");
        case Remote::Key_Back:
            return QString("<");
        case Remote::Key_Forward:
            return QString(">");
        case Remote::Key_Up:
            return tr("Up");
        case Remote::Key_Down:
            return tr("Down");
        case Remote::Key_Left:
            return tr("Left");
        case Remote::Key_Right:
            return tr("Right");
        case Remote::Key_Enter:
            return tr("OK");
        case Remote::Key_VolumeUp:
            return tr("Vol+");
        case Remote::Key_VolumeDown:
            return tr("Vol-");
        case Remote::Key_VolumeMute:
            return tr("Mute");
        case Remote::Key_BouquetUp:
            return tr("Bouq+");
        case Remote::Key_BouquetDown:
            return tr("Bouq-");
        case Remote::Key_Info:
            return tr("Info");
        case Remote::Key_Menu:
            return tr("Menu");
        case Remote::Key_Exit:
            return tr("Exit");
        case Remote::Key_Audio:
            return tr("Audio");
        case Remote::Key_Video:
            return tr("Video");
        case Remote::Key_Red:
            return tr("Red");
        case Remote::Key_Green:
            return tr("Green");
        case Remote::Key_Yellow:
            return tr("Yellow");
        case Remote::Key_Blue:
            return tr("Blue");
        case Remote::Key_Tv:
            return tr("TV");
        case Remote::Key_Radio:
            return tr("Radio");
        case Remote::Key_Text:
            return tr("Text");
        case Remote::Key_Help:
            return tr("Help");
        case Remote::Key_Power:
            return tr("Power");
        default:
            return QString();
        }
    }
};

#endif // REMOTEKEYS_H
