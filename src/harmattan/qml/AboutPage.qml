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
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

MyPage {
    id: root
    
    title: qsTr("About")
    tools: ToolBarLayout {
        BackToolIcon {}
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

            Image {
                id: icon

                x: Math.floor((parent.width - width) / 2)
                source: "file:///usr/share/icons/hicolor/80x80/apps/tuxr.png"
            }

            Label {
                id: titleLabel

                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pixelSize: UI.FONT_XLARGE
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
                text: "<font color ='" + UI.COLOR_INVERTED_FOREGROUND + "'>" + qsTr("Contact")
                + ": </font><u>showarth@marxoft.co.uk</u>"
                link: "mailto:showarth@marxoft.co.uk?subject=TuxR " + VERSION_NUMBER + " for N9"
                onClicked: root.accept()
            }
        }
    }
    
    ScrollDecorator {
        flickableItem: flickable
    }
}
