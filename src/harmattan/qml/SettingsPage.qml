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

    title: qsTr("Settings")
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
            }
            spacing: UI.PADDING_DOUBLE

            ValueSelector {
                width: parent.width
                title: qsTr("Screen orientation")
                model: ScreenOrientationModel {}
                value: settings.screenOrientation
                onValueChanged: settings.screenOrientation = value
            }            
            
            Label {
                x: UI.PADDING_DOUBLE
                font.bold: true
                text: qsTr("Active color")
            }

            Flow {
                x: UI.PADDING_DOUBLE
                width: parent.width - UI.PADDING_DOUBLE * 2
                spacing: UI.PADDING_DOUBLE

                Repeater {
                    model: ActiveColorModel {}

                    Rectangle {
                        width: 50
                        height: 50
                        color: value
                        border.color: "white"
                        border.width: color == settings.activeColor ? 2 : 0

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                settings.activeColor = value;
                                settings.activeColorString = "color" + (index + 2).toString();
                            }
                        }
                    }
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }
}
