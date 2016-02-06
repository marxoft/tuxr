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
    
    title: qsTr("Remotes")
    height: 360
    
    ListView {
        id: view
        
        anchors {
            left: parent.left
            right: button.left
            rightMargin: platformStyle.paddingMedium
            top: parent.top
            bottom: parent.bottom
        }
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        model: RemoteConfigModel {
            id: remoteModel
        }
        delegate: RemoteDelegate {
            onClicked: {
                if (settings.currentRemote != id) {
                    settings.currentRemote = id;
                    infoBanner.showMessage(qsTr("Remote selected"));
                }
            }
            onPressAndHold: contextMenu.popup()
        }
    }
    
    Menu {
        id: contextMenu
        
        MenuItem {
            text: qsTr("Edit")
            onTriggered: {
                var dialog = loader.load(remoteDialog, root);
                var remote = remoteModel.itemData(view.currentIndex);
                dialog.remoteId = remote.id;
                dialog.name = remote.name;
                dialog.address = remote.address;
                dialog.username = remote.username;
                dialog.password = remote.password;
                dialog.open();
            }
        }
        
        MenuItem {
            text: qsTr("Remove")
            onTriggered: {
                if (remotes.remove(remoteModel.data(view.currentIndex, "id"))) {
                    infoBanner.showMessage(qsTr("Remote removed"));
                }
            }
        }
    }
    
    Label {
        id: label
        
        anchors.fill: view
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: platformStyle.disabledTextColor
        font.pointSize: platformStyle.fontSizeLarge
        text: qsTr("No remotes")
        visible: remoteModel.count == 0
    }
    
    Button {
        id: button
        
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        style: DialogButtonStyle {}
        text: qsTr("New")
        onClicked: loader.open(remoteDialog, root)
    }
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: remoteDialog
        
        RemoteDialog {}
    }
    
    Component.onCompleted: remoteModel.reload()
}
