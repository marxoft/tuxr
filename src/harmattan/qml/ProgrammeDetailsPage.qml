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

MyPage {
    id: root
    
    property alias programmeTitle: programmeTitleLabel.text
    property string channelId
    property alias channelTitle: channelTitleLabel.text
    property variant startTime
    property variant endTime
    property alias description: descriptionLabel.text
    
    title: qsTr("Programme details")
    tools: ToolBarLayout {
        BackToolIcon {}
        
        MyToolIcon {
            platformIconId: "toolbar-clock"
            enabled: (channelId) && (channelTitle)
            onClicked: {
                var dialog = loader.load(timerDialog);
                dialog.channelId = channelId;
                dialog.channelTitle = channelTitle;
                dialog.startTime = startTime;
                dialog.endTime = endTime;
                dialog.open();
            }
        }
    }
    
    Flickable {
        id: flickable
        
        anchors.fill: parent
        contentHeight: column.height + UI.PADDING_DOUBLE
        
        Column {
            id: column
            
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: UI.PADDING_DOUBLE
            }
            spacing: UI.PADDING_DOUBLE
            
            Label {
                width: parent.width
                font.bold: true
                text: qsTr("Title")
            }
            
            Label {
                id: programmeTitleLabel

                width: parent.width
            }
            
            Label {
                width: parent.width
                font.bold: true
                text: qsTr("Channel")
            }
            
            Label {
                id: channelTitleLabel

                width: parent.width
            }
            
            Label {
                width: parent.width
                font.bold: true
                text: qsTr("Start time")
            }
            
            Label {
                id: startTimeLabel

                width: parent.width
                text: Qt.formatDateTime(startTime, DATE_TIME_FORMAT)
            }
            
            Label {
                width: parent.width
                font.bold: true
                text: qsTr("End time")
            }
            
            Label {
                id: endTimeLabel

                width: parent.width
                text: Qt.formatDateTime(endTime, DATE_TIME_FORMAT)
            }
            
            Label {
                width: parent.width
                font.bold: true
                text: qsTr("Description")
            }
            
            Label {
                id: descriptionLabel

                width: parent.width
            }
        }
    }
    
    ScrollDecorator {
        flickableItem: flickable
    }
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: timerDialog
        
        ProgrammeTimerDialog {}
    }
}
