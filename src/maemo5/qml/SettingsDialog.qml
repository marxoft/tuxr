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
import TuxR 1.0

Dialog {
    id: root

    title: qsTr("Settings")
    height: column.height + platformStyle.paddingMedium

    Column {
        id: column

        anchors {
            left: parent.left
            right: button.left
            top: parent.top
            rightMargin: platformStyle.paddingMedium
        }
        spacing: platformStyle.paddingMedium

        Button {
            width: parent.width
            text: qsTr("Remotes")
            onClicked: popupManager.open(remotesDialog, root)
        }

        CheckBox {
            width: parent.width
            text: qsTr("Use volume keys")
            checked: settings.volumeKeysEnabled
            onCheckedChanged: settings.volumeKeysEnabled = checked
        }

        ListSelectorButton {
            width: parent.width
            text: qsTr("Screen orientation")
            model: ScreenOrientationModel {}
            value: settings.screenOrientation
            onSelected: settings.screenOrientation = value
        }
    }

    Button {
        id: button

        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        style: DialogButtonStyle {}
        text: qsTr("Done")
        onClicked: root.accept()
    }

    Component {
        id: remotesDialog

        RemotesDialog {}
    }

    contentItem.states: State {
        name: "Portrait"
        when: screen.currentOrientation == Qt.WA_Maemo5PortraitOrientation

        AnchorChanges {
            target: column
            anchors.right: parent.right
        }

        PropertyChanges {
            target: column
            anchors.rightMargin: 0
        }

        PropertyChanges {
            target: button
            width: parent.width
        }

        PropertyChanges {
            target: root
            height: column.height + button.height + platformStyle.paddingMedium * 2
        }
    }
}
