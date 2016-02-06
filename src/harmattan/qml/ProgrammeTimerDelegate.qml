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
import com.nokia.meego 1.0
import TuxR 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

ListItem {
    id: root
    
    Label {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: UI.PADDING_DOUBLE
        }
        elide: Text.ElideRight
        font.bold: true
        text: channelTitle
    }
    
    Label {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: UI.PADDING_DOUBLE
        }
        font.pixelSize: UI.FONT_SMALL
        font.family: UI.FONT_FAMILY_LIGHT
        elide: Text.ElideRight
        text: (eventType == ProgrammeTimer.RecurringEvent
              ? Qt.formatTime(startTime, TIME_FORMAT) + " - " + Qt.formatTime(endTime, TIME_FORMAT)
              : Qt.formatDateTime(startTime, DATE_TIME_FORMAT) + " - " + Qt.formatDateTime(endTime, DATE_TIME_FORMAT))
    }
}
