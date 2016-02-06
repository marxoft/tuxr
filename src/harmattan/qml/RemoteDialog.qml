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
                    font.pixelSize: UI.FONT_XLARGE
                    text: remoteId ? qsTr("Edit remote") : qsTr("Add remote")
                }
                
                Image {
                    width: parent.width
                    source: "image://theme/meegotouch-groupheader-inverted-background"
                }
                
                ValueSelector {
                    id: remoteTypeSelector
                    
                    x: -UI.PADDING_DOUBLE
                    width: parent.width + UI.PADDING_DOUBLE * 2
                    title: qsTr("Remote type")
                    model: RemoteTypeModel {
                        id: remoteTypeModel
                    }
                }
                
                Label {
                    width: parent.width
                    font.bold: true
                    text: qsTr("Display name")
                }
                
                MyTextField {
                    id: nameField
                    
                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    actionLabel: qsTr("Next")
                    onAccepted: addressField.forceActiveFocus()
                }
                
                Label {
                    width: parent.width
                    font.bold: true
                    text: qsTr("IP address")
                }
                
                MyTextField {
                    id: addressField
                    
                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    onAccepted: platformCloseSoftwareInputPanel()
                }
                
                Label {
                    width: parent.width
                    font.bold: true
                    text: qsTr("Username")
                }
                
                MyTextField {
                    id: usernameField
                    
                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    actionLabel: qsTr("Next")
                    onAccepted: passwordField.forceActiveFocus()
                }
                
                Label {
                    width: parent.width
                    font.bold: true
                    text: qsTr("Password")
                }
                
                MyTextField {
                    id: passwordField
                    
                    width: parent.width
                    echoMode: TextInput.Password
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    onAccepted: platformCloseSoftwareInputPanel()
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
