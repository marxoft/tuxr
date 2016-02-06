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
    
    property alias text: textField.text
    property alias keys: view.model
        
    acceptButtonText: (textField.text) && (view.count > 0) ? qsTr("Done") : ""
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
                    id: titleLabel
                    
                    width: parent.width
                    font.pixelSize: UI.FONT_XLARGE
                    text: qsTr("Add macro")
                }
                
                Image {
                    width: parent.width
                    source: "image://theme/meegotouch-groupheader-inverted-background"
                }
                
                Label {
                    width: parent.width
                    font.bold: true
                    text: qsTr("Text")
                }
                
                MyTextField {
                    id: textField
                    
                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    onAccepted: platformCloseSoftwareInputPanel()
                }
                
                SeparatorLabel {
                    width: parent.width
                    text: qsTr("Keys")
                }
                
                MyButton {
                    id: keyButton
                    
                    x: Math.floor((parent.width - width) / 2)
                    text: qsTr("Add key")
                    onClicked: keyDialog.open()
                }
                
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
                    
                    x: -UI.PADDING_DOUBLE
                    width: parent.width + UI.PADDING_DOUBLE * 2
                    height: (64 + UI.PADDING_DOUBLE * 2) * count
                    interactive: false
                    model: []
                    delegate: ListItem {                        
                        Label {
                            anchors {
                                left: parent.left
                                right: parent.right
                                verticalCenter: parent.verticalCenter
                                margins: UI.PADDING_DOUBLE
                            }
                            font.bold: true
                            elide: Text.ElideRight
                            text: remoteKeys.text(modelData)
                        }
                        
                        onClicked: contextMenu.open()
                        onPressAndHold: contextMenu.open()
                    }
                }
            }
            
            ContextMenu {
                id: contextMenu
                
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
	id: keyDialog
	
        titleText: qsTr("Add key")
        model: RemoteKeyModel {}
        onAccepted: view.append(value)
	onStatusChanged: if (status == DialogStatus.Opening) selectedIndex = -1;
    }
    
    states: State {
        name: "edit"
        
        PropertyChanges {
            target: titleLabel
            text: qsTr("Edit macro")
        }
    }
}
