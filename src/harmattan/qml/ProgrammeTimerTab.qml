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

Tab {
    id: root
    
    property QtObject timers: instantiator.createProgrammeTimers(settings.currentRemote, root)
    
    title: qsTr("Timers")
    tools: ToolBarLayout {
        MyToolIcon {
            id: newButton
            
            platformIconId: "toolbar-add"
            onClicked: loader.open(timerDialog)
        }
        
        MyToolIcon {
            id: clearButton
            
            platformIconId: "toolbar-delete"
            onClicked: deleteMenu.open()
        }
        
        MyToolIcon {
            id: reloadButton
            
            platformIconId: "toolbar-refresh"
            onClicked: timerModel.reload()
        }
        
        MyToolIcon {
            id: menuButton
            
            platformIconId: "toolbar-view-menu"
            onClicked: appMenu.open()
        }
    }
    
    Menu {
        id: deleteMenu
        
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
    
    ListView {
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
    
    ContextMenu {
        id: contextMenu
        
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
            margins: UI.PADDING_DOUBLE
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: UI.COLOR_INVERTED_SECONDARY_FOREGROUND
        font.bold: true
        font.pixelSize: 40
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
            onAccepted: timerModel.reload()
        }
    }
    
    Component.onCompleted: timerModel.reload()
}
