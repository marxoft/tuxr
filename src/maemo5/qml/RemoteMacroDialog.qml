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
    
    property alias text: textField.text
    property alias keys: view.model
    property alias state: stateGroup.state
    
    title: qsTr("Add macro")
    height: 360
    
    ListView {
        id: view
        
        function append(key) {
            var k = model;
            k.push(key);
            model = k;
        }
        
        function remove(i) {
            var k = model;
            k.splice(i, 1);
            model = k;
        }
        
        function clear() {
            model = [];
        }
        
        anchors {
            left: parent.left
            right: buttonColumn.left
            rightMargin: platformStyle.paddingMedium
            top: parent.top
            bottom: label.top
            bottomMargin: platformStyle.paddingMedium
        }
        clip: true
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        model: []
        delegate: ListItem {                        
            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: platformStyle.paddingMedium
                }
                elide: Text.ElideRight
                text: remoteKeys.text(modelData)
            }
            
            onClicked: popupManager.open(contextMenu, root)
            onPressAndHold: popupManager.open(contextMenu, root)
        }
        
        Label {
            anchors {
                fill: parent
                margins: platformStyle.paddingMedium
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            color: platformStyle.disabledTextColor
            font.pointSize: platformStyle.fontSizeLarge
            text: qsTr("No keys")
            visible: view.count == 0
        }
    }
    
    Label {
        id: label
        
        anchors {
            left: view.left
            right: view.right
            bottom: textField.top
            bottomMargin: platformStyle.paddingMedium
        }
        text: qsTr("Text")
    }
    
    TextField {
        id: textField
        
        anchors {
            left: view.left
            right: view.right
            bottom: parent.bottom
        }
    }
    
    DialogButtonStyle {
        id: dialogButtonStyle
    }
    
    Column {
        id: buttonColumn
        
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        spacing: platformStyle.paddingMedium
        
        Button {
            id: keyButton
            
            text: qsTr("Add key")
            style: dialogButtonStyle
            onClicked: popupManager.open(keySelector, root)
        }
        
        Button {
            id: acceptButton
            
            text: qsTr("Done")
            style: dialogButtonStyle
            enabled: (textField.text) && (view.count > 0)
            onClicked: root.accept()
        }
    }
    
    Component {
        id: keySelector

        ListPickSelector {
            title: qsTr("Add key")
            model: RemoteKeyModel {
                id: keyModel
            }
            textRole: "name"
            onSelected: view.append(keyModel.data(currentIndex, "value"))
        }
    }

    Component {
        id: contextMenu

        Menu {
            MenuItem {
                text: qsTr("Remove key")
                onTriggered: view.remove(view.currentIndex)
            }
            
            MenuItem {
                text: qsTr("Remove all keys")
                onTriggered: view.clear()
            }
        }
    }
    
    StateGroup {
        id: stateGroup
        
        states: State {
            name: "edit"
            
            PropertyChanges {
                target: root
                title: qsTr("Edit macro")
            }
        }
    }

    contentItem.states: State {
        name: "Portrait"
        when: screen.currentOrientation == Qt.WA_Maemo5PortraitOrientation

        AnchorChanges {
            target: view
            anchors.right: parent.right
        }

        PropertyChanges {
            target: view
            anchors.rightMargin: 0
        }

        AnchorChanges {
            target: textField
            anchors.bottom: buttonColumn.top
        }

        PropertyChanges {
            target: textField
            anchors.bottomMargin: platformStyle.paddingMedium
        }

        PropertyChanges {
            target: dialogButtonStyle
            buttonWidth: parent.width
        }

        PropertyChanges {
            target: root
            height: 680
        }
    }
    
    onStatusChanged: if (status == DialogStatus.Open) textField.forceActiveFocus();
}
