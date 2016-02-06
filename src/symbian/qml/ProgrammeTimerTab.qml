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

Tab {
    id: root
    
    property QtObject timers: instantiator.createProgrammeTimers(settings.currentRemote, root)
    
    title: qsTr("Timers")
    tools: ToolBarLayout {
        MyToolButton {
            id: newButton
            
            iconSource: "toolbar-add"
            toolTip: qsTr("Add timer")
            onClicked: loader.open(timerDialog)
        }
        
        MyToolButton {
            id: clearButton
            
            iconSource: "toolbar-delete"
            toolTip: qsTr("Clear timers")
            onClicked: deleteMenu.open()
        }
        
        MyToolButton {
            id: reloadButton
            
            iconSource: "toolbar-refresh"
            toolTip: qsTr("Reload")
            onClicked: timerModel.reload()
        }
        
        MyToolButton {
            id: menuButton
            
            iconSource: "toolbar-view-menu"
            toolTip: qsTr("Options")
            onClicked: appMenu.open()
        }
    }
    
    MyMenu {
        id: deleteMenu
        
        focusItem: view
        
        MenuLayout {
            MenuItem {
                text: qsTr("Clear completed timers")
                onClicked: timers.cleanupProgrammeTimers()
            }
            
            MenuItem {
                text: qsTr("Clear all timers")
                onClicked: timers.clearProgrammeTimers()
            }
        }
    }
    
    MyListView {
        id: view
        
        anchors.fill: parent
        model: ProgrammeTimerModel {
            id: timerModel
            
            timers: instantiator.createProgrammeTimerList(settings.currentRemote, timerModel)
            onStatusChanged: {
                switch (status) {
                case Request.Active: {
                    root.showProgressIndicator = true;
                    reloadButton.enabled = false;
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
                reloadButton.enabled = true;
                label.visible = (count == 0);
            }
        }
        delegate: ProgrammeTimerDelegate {
            onClicked: {
                var dialog = loader.load(timerDialog);
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
                    var dialog = loader.load(timerDialog);
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
                onClicked: {
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
        text: qsTr("No timers")
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
            focusItem: view
            onAccepted: timerModel.reload()
        }
    }
    
    Component.onCompleted: timerModel.reload()
}
