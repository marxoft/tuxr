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
    
    property string remoteId
    property alias remoteType: remoteTypeButton.value
    property alias name: nameField.text
    property alias address: addressField.text
    property alias username: usernameField.text
    property alias password: passwordField.text
    
    title: remoteId ? qsTr("Edit remote") : qsTr("New remote")
    height: Math.min(360, column.height + platformStyle.paddingMedium)
    
    Flickable {
        id: flickable
        
        anchors {
            left: parent.left
            right: button.left
            rightMargin: platformStyle.paddingMedium
            top: parent.top
            bottom: parent.bottom
        }
        contentHeight: column.height
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        
        Column {
            id: column
            
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            spacing: platformStyle.paddingMedium
            
            ListSelectorButton {
                id: remoteTypeButton
                
                width: parent.width
                text: qsTr("Remote type")
                model: RemoteTypeModel {
                    id: remoteTypeModel
                }
                value: remoteType
                onSelected: remoteType = value
            }
            
            Label {
                width: parent.width
                text: qsTr("Display name")
            }
            
            TextField {
                id: nameField
                
                width: parent.width
            }
            
            Label {
                width: parent.width
                text: qsTr("IP address")
            }
            
            TextField {
                id: addressField
                
                width: parent.width
            }
            
            Label {
                width: parent.width
                text: qsTr("Username")
            }
            
            TextField {
                id: usernameField
                
                width: parent.width
            }
            
            Label {
                width: parent.width
                text: qsTr("Password")
            }
            
            TextField {
                id: passwordField
                
                width: parent.width
                echoMode: TextInput.Password
            }
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
        enabled: (name) && (address)
        onClicked: root.accept()
    }

    contentItem.states: State {
        name: "Portrait"
        when: screen.currentOrientation == Qt.WA_Maemo5PortraitOrientation

        AnchorChanges {
            target: flickable
            anchors.right: parent.right
            anchors.bottom: button.top
        }

        PropertyChanges {
            target: flickable
            anchors.rightMargin: 0
            anchors.bottomMargin: platformStyle.paddingMedium
        }

        PropertyChanges {
            target: button
            width: parent.width
        }

        PropertyChanges {
            target: root
            height: Math.min(680, column.height + button.height + platformStyle.paddingMedium * 2)
        }
    }

    onAccepted: {
        if (remoteId) {
            remotes.setRemoteType(remoteId, remoteType);
            remotes.setName(remoteId, name);
            remotes.setAddress(remoteId, address);
            remotes.setUsername(remoteId, username);
            remotes.setPassword(remoteId, password);
            infoBanner.showMessage(qsTr("Remote updated"));
        }
        else {
            remotes.add(remoteType, name, address, 0, username, password);
            infoBanner.showMessage(qsTr("Remote added"));
        }
    }
}
