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
    
    property alias text: textField.text
    property alias keys: view.model
        
    acceptButtonText: (textField.text) && (view.count > 0) ? qsTr("Done") : ""
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
                    id: titleLabel
                    
                    width: parent.width
                    font.pixelSize: platformStyle.fontSizeLarge
                    font.bold: true
                    text: qsTr("Add macro")
                    visible: !inputContext.visible
                }
                
                Rectangle {
                    width: parent.width
                    height: 1
                    color: platformStyle.colorDisabledMid
                    visible: !inputContext.visible
                }
                
                Label {
                    width: parent.width
                    text: qsTr("Text")
                    visible: textField.visible
                }
                
                MyTextField {
                    id: textField
                    
                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    visible: (!inputContext.visible) || (focus)
                }
                
                SeparatorLabel {
                    width: parent.width
                    text: qsTr("Keys")
                    visible: !inputContext.visible
                }
                
                MyButton {
                    id: keyButton
                    
                    x: Math.floor((parent.width - width) / 2)
                    text: qsTr("Add key")
                    visible: !inputContext.visible
                    onClicked: {
                        var dialog = keyDialog.createObject(root);
                        dialog.open();
                    }
                }
                
                MyListView {
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
                    
                    x: -platformStyle.paddingLarge
                    width: parent.width + platformStyle.paddingLarge * 2
                    height: (64 + platformStyle.paddingLarge * 2) * count
                    interactive: false
                    model: []
                    visible: !inputContext.visible
                    delegate: MyListItem {                        
                        MyListItemText {
                            anchors {
                                left: parent.left
                                right: parent.right
                                verticalCenter: parent.verticalCenter
                                margins: platformStyle.paddingLarge
                            }
                            role: "Title"
                            elide: Text.ElideRight
                            text: remoteKeys.text(modelData)
                        }
                        
                        onClicked: contextMenu.open()
                        onPressAndHold: contextMenu.open()
                    }
                }
            }
            
            MyContextMenu {
                id: contextMenu
                
                focusItem: view
                
                MenuLayout {
                    MenuItem {
                        text: qsTr("Remove key")
                        onClicked: view.remove(view.currentIndex)
                    }
                    
                    MenuItem {
                        text: qsTr("Remove all keys")
                        onClicked: view.clear()
                    }
                }
            }
        }
        
        ScrollDecorator {
            flickableItem: flickable
        }
    }
    
    ValueDialog {
        focusItem: flickable
        titleText: qsTr("Add key")
        model: RemoteKeyModel {}
        onAccepted: view.append(value)
    }
    
    states: State {
        name: "edit"
        
        PropertyChanges {
            target: titleLabel
            text: qsTr("Edit macro")
        }
    }
}
