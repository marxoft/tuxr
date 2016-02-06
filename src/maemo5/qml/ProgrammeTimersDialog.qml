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
    
    property QtObject timers: instantiator.createProgrammeTimers(settings.currentRemote, root)
    
    title: qsTr("Timers")
    height: 360
    
    ListView {
        id: view
        
        anchors {
            left: parent.left
            right: buttonColumn.left
            rightMargin: platformStyle.paddingMedium
            top: parent.top
            bottom: parent.bottom
        }
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        model: ProgrammeTimerModel {
            id: timerModel
            
            timers: instantiator.createProgrammeTimerList(settings.currentRemote, timerModel)
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
                
                root.showProgressIndicator = (timers.status == Request.Active);
                label.visible = (count == 0);
            }
        }
        delegate: ProgrammeTimerDelegate {
            onClicked: {
                var dialog = loader.load(timerDialog, root);
                var timer = timerModel.itemData(index);
                dialog.timerId = timer.id;
                dialog.eventType = timer.eventType;
                dialog.action = timer.action;
                dialog.afterAction = timer.afterAction;
                dialog.startTime = timer.startTime;
                dialog.endTime = timer.endTime;
                dialog.recurringDays = timer.recurringDays;
                dialog.channelId = timer.channelId;
                dialog.channelTitle = timer.channelTitle;
                dialog.description = timer.description;
                dialog.open();
            }
            onPressAndHold: contextMenu.popup()
        }
    }
    
    Menu {
        id: contextMenu
        
        MenuItem {
            text: qsTr("Edit")
            onTriggered: {
                var dialog = loader.load(timerDialog, root);
                var timer = timerModel.itemData(view.currentIndex);
                dialog.timerId = timer.id;
                dialog.eventType = timer.eventType;
                dialog.action = timer.action;
                dialog.afterAction = timer.afterAction;
                dialog.startTime = timer.startTime;
                dialog.endTime = timer.endTime;
                dialog.recurringDays = timer.recurringDays;
                dialog.channelId = timer.channelId;
                dialog.channelTitle = timer.channelTitle;
                dialog.description = timer.description;
                dialog.open();
            }
        }
        
        MenuItem {
            text: qsTr("Remove")
            onTriggered: {
                var item = timerModel.itemData(view.currentIndex);
                timer.id = item.id;
                timer.eventType = item.eventType;
                timer.action = item.action;
                timer.afterAction = item.afterAction;
                timer.startTime = item.startTime;
                timer.endTime = item.endTime;
                timer.recurringDays = item.recurringDays;
                timer.channelId = item.channelId;
                timer.channelTitle = item.channelTitle;
                timer.description = item.description;
                timers.removeProgrammeTimer(timer);
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
        text: qsTr("No timers")
    }
    
    Menu {
        id: deleteMenu
        
        MenuItem {
            text: qsTr("Clear completed timers")
            onTriggered: timers.cleanupProgrammeTimers()
        }
        
        MenuItem {
            text: qsTr("Clear all timers")
            onTriggered: timers.clearProgrammeTimers()
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
            id: newButton
            
            style: dialogButtonStyle
            text: qsTr("New")
            activeFocusOnPress: false
            onClicked: loader.open(timerDialog, root)
        }
        
        Button {
            id: clearButton
            
            style: dialogButtonStyle
            text: qsTr("Clear")
            activeFocusOnPress: false
            onClicked: deleteMenu.popup()
        }
    }
    
    ProgrammeTimer {
        id: timer
    }
    
    Connections {
        target: timers
        onStatusChanged: {
            switch (timers.status) {
                case Request.Active:
                root.showProgressIndicator = true;
                return;
                case Request.Ready: {
                    infoBanner.showMessage(qsTr("Timer(s) removed"));
                    timerModel.reload();
                    break;
                }
                case Request.Error:
                infoBanner.showMessage(timers.errorString);
                break;
                default:
                break;
            }
            
            root.showProgressIndicator = (timerModel.status == Request.Active);
        }
    }
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: timerDialog
        
        ProgrammeTimerDialog {            
            onAccepted: timerModel.reload()
        }
    }
    
    onStatusChanged: {
        if (status == DialogStatus.Open) {
            timerModel.reload();
            view.forceActiveFocus();
        }
    }
}
