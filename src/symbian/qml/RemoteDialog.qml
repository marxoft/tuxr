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
import TuxR 1.0

MySheet {
    id: root
    
    property string remoteId
    property alias remoteType: remoteTypeSelector.value
    property alias name: nameField.text
    property alias address: addressField.text
    property alias username: usernameField.text
    property alias password: passwordField.text
    
    acceptButtonText: (name) && (address) ? qsTr("Done") : ""
    rejectButtonText: qsTr("Cancel")
    content: Item {
        anchors.fill: parent
        clip: true
        
        KeyNavFlickable {
            id: flickable
            
            anchors.fill: parent
            contentHeight: inputContext.visible ? height : column.height + platformStyle.paddingLarge
            
            Column {
                id: column
                
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    margins: platformStyle.paddingLarge
                }
                spacing: platformStyle.paddingLarge
                
                Label {
                    width: parent.width
                    font.pixelSize: platformStyle.fontSizeLarge
                    font.bold: true
                    text: remoteId ? qsTr("Edit remote") : qsTr("Add remote")
                    visible: !inputContext.visible
                }
                
                Rectangle {
                    width: parent.width
                    height: 1
                    color: platformStyle.colorDisabledMid
                    visible: !inputContext.visible
                }
                
                ValueSelector {
                    id: remoteTypeSelector
                    
                    x: -platformStyle.paddingLarge
                    width: parent.width + platformStyle.paddingLarge * 2
                    focusItem: flickable
                    title: qsTr("Remote type")
                    model: RemoteTypeModel {
                        id: remoteTypeModel
                    }
                    visible: !inputContext.visible
                }
                
                Label {
                    width: parent.width
                    text: qsTr("Display name")
                    visible: nameField.visible
                }
                
                MyTextField {
                    id: nameField
                    
                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    visible: (!inputContext.visible) || (focus)
                }
                
                Label {
                    width: parent.width
                    text: qsTr("IP address")
                    visible: addressField.visible
                }
                
                MyTextField {
                    id: addressField
                    
                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    visible: (!inputContext.visible) || (focus)
                }
                
                Label {
                    width: parent.width
                    text: qsTr("Username")
                    visible: usernameField.visible
                }
                
                MyTextField {
                    id: usernameField
                    
                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    visible: (!inputContext.visible) || (focus)
                }
                
                Label {
                    width: parent.width
                    text: qsTr("Password")
                    visible: passwordField.visible
                }
                
                MyTextField {
                    id: passwordField
                    
                    width: parent.width
                    echoMode: TextInput.Password
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    visible: (!inputContext.visible) || (focus)
                }
            }
        }
        
        ScrollDecorator {
            flickableItem: flickable
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
