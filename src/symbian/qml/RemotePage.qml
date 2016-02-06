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

MyPage {
    id: root
    
    title: qsTr("Remotes")
    tools: ToolBarLayout {
        BackToolButton {}
        
        MyToolButton {
            iconSource: "toolbar-add"
            toolTip: qsTr("Add remote")
            onClicked: loader.open(remoteDialog)
        }
    }
    
    MyListView {
        id: view
        
        anchors.fill: parent
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
            onPressAndHold: contextMenu.open()
        }
    }
    
    ScrollDecorator {
        flickableItem: view
    }
    
    MyContextMenu {
        id: contextMenu
        
        focusItem: view
        
        MenuLayout {
            MenuItem {
                text: qsTr("Edit")
                onClicked: {
                    var dialog = loader.load(remoteDialog);
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
                onClicked: {
                    if (remotes.remove(remoteModel.data(view.currentIndex, "id"))) {
                        infoBanner.showMessage(qsTr("Remote removed"));
                    }
                }
            }
        }
    }
    
    Label {
        id: label
        
        anchors {
            fill: parent
            margins: platformStyle.paddingLarge
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: platformStyle.colorNormalMid
        font.bold: true
        font.pixelSize: 32
        text: qsTr("No remotes")
        visible: remoteModel.count == 0
    }
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: remoteDialog
        
        RemoteDialog {
            focusItem: view
        }
    }
    
    Component.onCompleted: remoteModel.reload()
}
