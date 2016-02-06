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

import QtQuick 1.1
import com.nokia.symbian 1.1
import TuxR 1.0

MyListItem {
    id: root

    height: 56 + platformStyle.paddingLarge * 2
    
    MyListItemText {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: platformStyle.paddingLarge
        }
        role: "Title"

        elide: Text.ElideRight
        text: channelTitle
    }
    
    MyListItemText {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: platformStyle.paddingLarge
        }
        role: "SubTitle"
        elide: Text.ElideRight
        text: (eventType == ProgrammeTimer.RecurringEvent
               ? Qt.formatTime(startTime, TIME_FORMAT) + " - " + Qt.formatTime(endTime, TIME_FORMAT)
               : Qt.formatDateTime(startTime, DATE_TIME_FORMAT) + " - " + Qt.formatDateTime(endTime, DATE_TIME_FORMAT))
    }
}
