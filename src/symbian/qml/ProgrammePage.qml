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
    
    property string serviceId
    
    title: qsTr("EPG")
    tools: ToolBarLayout {
        BackToolButton {}
    }
    
    MyListView {
        id: view
        
        anchors.fill: parent
        model: ProgrammeModel {
            id: programmeModel
            
            programmes: instantiator.createProgrammeList(settings.currentRemote, programmeModel)
            onStatusChanged: {
                switch (status) {
                case Request.Active: {
                    root.showProgressIndicator = true;
                    label.visible = false;
                    return;
                }
                case Request.Error:
                    infoBanner.showMessage(errorString);
                    break;
                default:
                    break;
                }
                
                root.showProgressIndicator = false;
                label.visible = (count == 0);
            }
        }
        delegate: ProgrammeDelegate {
            onClicked: {
                var programme = programmeModel.itemData(index);
                var properties = {programmeTitle: programme.title, channelId: programme.channelId,
                                  channelTitle: programme.channelTitle, startTime: programme.startTime,
                                  endTime: programme.endTime, description: programme.description};
                appWindow.pageStack.push(Qt.resolvedUrl("ProgrammeDetailsPage.qml"), properties);
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
                text: qsTr("Show details")
                onClicked: {
                    var programme = programmeModel.itemData(view.currentIndex);
                    var properties = {programmeTitle: programme.title, channelId: programme.channelId,
                                      channelTitle: programme.channelTitle, startTime: programme.startTime,
                                      endTime: programme.endTime, description: programme.description};
                    appWindow.pageStack.push(Qt.resolvedUrl("ProgrammeDetailsPage.qml"), properties);
                }
            }
            
            MenuItem {
                text: qsTr("Set timer")
                onClicked: {
                    var dialog = loader.load(timerDialog);
                    var programme = programmeModel.itemData(view.currentIndex);
                    dialog.startTime = programme.startTime;
                    dialog.endTime = programme.endTime;
                    dialog.channelId = programme.channelId;
                    dialog.channelTitle = programme.channelTitle;
                    dialog.open();
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
        text: qsTr("No programmes")
    }
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: timerDialog
        
        ProgrammeTimerDialog {
            focusItem: view
        }
    }
    
    onServiceIdChanged: programmeModel.reload(serviceId)
    
    Component.onCompleted: if ((serviceId) && (programmeModel.count == 0)) programmeModel.reload(serviceId);
}
