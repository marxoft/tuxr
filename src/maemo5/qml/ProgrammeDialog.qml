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
    
    property string serviceId
    
    title: qsTr("EPG")
    height: 360
    
    ListView {
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
                var dialog = loader.load(detailsDialog, root);
                var programme = programmeModel.itemData(index);
                dialog.programmeTitle = programme.title;
                dialog.channelId = programme.channelId;
                dialog.channelTitle = programme.channelTitle;
                dialog.startTime = programme.startTime;
                dialog.endTime = programme.endTime;
                dialog.description = programme.description;
                dialog.open();
            }
            onPressAndHold: contextMenu.popup()
        }
    }
    
    Menu {
        id: contextMenu
        
        MenuItem {
            text: qsTr("Show details")
            onTriggered: {
                var dialog = loader.load(detailsDialog, root);
                var programme = programmeModel.itemData(view.currentIndex);
                dialog.programmeTitle = programme.title;
                dialog.channelId = programme.channelId;
                dialog.channelTitle = programme.channelTitle;
                dialog.startTime = programme.startTime;
                dialog.endTime = programme.endTime;
                dialog.description = programme.description;
                dialog.open();
            }
        }
        
        MenuItem {
            text: qsTr("Set timer")
            onTriggered: {
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
    
    Label {
        id: label
        
        anchors {
            fill: view
            margins: platformStyle.paddingMedium
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: platformStyle.disabledTextColor
        font.pointSize: platformStyle.fontSizeLarge
        text: qsTr("No programmes")
    }
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: detailsDialog
        
        ProgrammeDetailsDialog {}
    }
    
    Component {
        id: timerDialog
        
        ProgrammeTimerDialog {}
    }
    
    onServiceIdChanged: if (status == DialogStatus.Open) programmeModel.reload(serviceId);
    
    onStatusChanged: if ((status == DialogStatus.Open) && (serviceId)
                         && (programmeModel.count == 0)) programmeModel.reload(serviceId);
}
