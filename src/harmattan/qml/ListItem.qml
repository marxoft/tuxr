/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 3, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

import QtQuick 1.1
import com.nokia.meego 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Item {
    id: root

    default property alias content: contentItem.data
    property bool subItemIndicator: false
    property bool isSelected: false

    signal clicked
    signal pressAndHold

    height: 64 + UI.PADDING_DOUBLE * 2
    width: parent ? parent.width : screen.displayWidth
    opacity: enabled ? UI.OPACITY_ENABLED : UI.OPACITY_DISABLED

    Loader {
        id: highlightLoader
        
        anchors {
            fill: parent
            topMargin: 1
            bottomMargin: 1
        }
        sourceComponent: (mouseArea.pressed) || (root.isSelected) ? highlight : undefined
    }

    Component {
        id: highlight

        Rectangle {
            anchors.fill: parent
            color: root.isSelected ? settings.activeColor : UI.COLOR_INVERTED_SECONDARY_FOREGROUND
            opacity: 0.5
        }
    }

    Loader {
        id: indicatorLoader
        
        anchors {
            right: parent.right
            rightMargin: UI.PADDING_DOUBLE
            verticalCenter: parent.verticalCenter
        }
        sourceComponent: root.subItemIndicator ? indicator : undefined
    }

    Component {
        id: indicator

        Image {
            source: "image://theme/icon-m-common-drilldown-arrow-inverse"
        }
    }
    
    Item {
        id: contentItem
        
        anchors {
            left: parent.left
            right: indicatorLoader.item ? indicatorLoader.left : parent.right
            top: parent.top
            bottom: parent.bottom
        }
    }

    MouseArea {
        id: mouseArea

        z: 1
        anchors.fill: parent
        enabled: root.enabled
        onClicked: root.clicked()
        onPressed: {
            if (root.ListView.view) {
                root.ListView.view.currentIndex = -1;
                root.ListView.view.currentIndex = index;
            }
        }
        onPressAndHold: root.pressAndHold()
    }
}
