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

import QtQuick 1.0
import org.hildon.components 1.0

Dialog {
    id: root
    
    title: qsTr("About")
    height: Math.min(360, column.height + platformStyle.paddingMedium)
    
    Flickable {
        id: flickable
        
        anchors.fill: parent
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        contentHeight: column.height
        
        Column {
            id: column
            
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            spacing: platformStyle.paddingMedium
            
            Row {
                width: parent.width
                spacing: platformStyle.paddingMedium
                
                Image {
                    width: 64
                    height: 64
                    source: "file:///usr/share/icons/hicolor/64x64/apps/tuxr.png"
                    smooth: true
                }

                Label {
                    height: 64
                    font.bold: true
                    font.pointSize: platformStyle.fontSizeLarge
                    verticalAlignment: Text.AlignVCenter
                    text: "TuxR " + VERSION_NUMBER
                }
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap
                text: qsTr("A remote control application for Linux-based set-top boxes.")
                      + "<br><br>"
                      + qsTr("Keyboard shortcuts:")
                      + "<br><br>"
                      + qsTr("Left/Right: Switch between Remote/Zap views.")
                      + "<br>"
                      + qsTr("Volume up/down: Volume control and fast service nagivation (if enabled).")
                      + "<br>"
                      + qsTr("Ctrl+P: Show screenshot.")
                      + "<br>"
                      + qsTr("Ctrl+T: Show timers.")
                      + "<br>"
                      + qsTr("Ctrl+S: Show settings dialog.")
                      + "<br>"
                      + qsTr("Ctrl+H: Show about dialog.")
                      + "<br><br>"
                      + qsTr("Remote macros:")
                      + "<br><br>"
                      + qsTr("Perform a long-press in empty space to edit/remove an existing macro.")
                      + "<br><br>&copy; Stuart Howarth 2016"
            }
        }
    }

    contentItem.states: State {
        name: "Portrait"
        when: screen.currentOrientation == Qt.WA_Maemo5PortraitOrientation

        PropertyChanges {
            target: root
            height: Math.min(680, column.height + platformStyle.paddingMedium)
        }
    }
}
