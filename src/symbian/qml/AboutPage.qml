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

MyPage {
    id: root
    
    title: qsTr("About")
    tools: ToolBarLayout {
        BackToolButton {}
    }
    
    MyFlickable {
        id: flickable
        
        anchors.fill: parent
        contentHeight: column.height + platformStyle.paddingLarge
        
        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: platformStyle.paddingLarge
            }
            spacing: platformStyle.paddingLarge

            Image {
                id: icon

                x: Math.floor((parent.width - width) / 2)
                source: "images/tuxr.svg"
            }

            Label {
                id: titleLabel

                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pixelSize: 32
                text: "TuxR " + VERSION_NUMBER
            }

            Label {
                id: aboutLabel

                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("A remote control application for Linux-based set-top boxes.<br><br>&copy; Stuart Howarth 2016")
            }

            LinkLabel {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                text: "<font color ='" + platformStyle.colorNormalLight + "'>" + qsTr("Contact")
                + ": </font><u>showarth@marxoft.co.uk</u>"
                link: "mailto:showarth@marxoft.co.uk?subject=TuxR " + VERSION_NUMBER + " for Symbian"
                onClicked: root.accept()
            }
        }
    }
    
    MyScrollBar {
        flickableItem: flickable
    }
}
