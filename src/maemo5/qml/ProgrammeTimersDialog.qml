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
                var t = timerModel.itemData(index);
                popupManager.open(timerDialog, root, {timerId: t.id, eventType: t.eventType, action: t.action,
                    afterAction: t.afterAction, startTime: t.startTime, endTime: t.endTime,
                    recurringDays: t.recurringDays, channelId: t.channelId, channelTitle: t.channelTitle,
                    description: t.description});
            }
            onPressAndHold: popupManager.open(contextMenu, root)
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
            onClicked: popupManager.open(timerDialog, root)
        }
        
        Button {
            id: clearButton
            
            style: dialogButtonStyle
            text: qsTr("Clear")
            activeFocusOnPress: false
            onClicked: popupManager.open(deleteMenu, root)
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

    Component {
        id: contextMenu

        Menu {
            MenuItem {
                text: qsTr("Edit")
                onTriggered: {
                    var t = timerModel.itemData(view.currentIndex);
                    popupManager.open(timerDialog, root, {timerId: t.id, eventType: t.eventType, action: t.action,
                        afterAction: t.afterAction, startTime: t.startTime, endTime: t.endTime,
                        recurringDays: t.recurringDays, channelId: t.channelId, channelTitle: t.channelTitle,
                        description: t.description});
                }
            }
            
            MenuItem {
                text: qsTr("Remove")
                onTriggered: {
                    var t = timerModel.itemData(view.currentIndex);
                    timer.id = t.id;
                    timer.eventType = t.eventType;
                    timer.action = t.action;
                    timer.afterAction = t.afterAction;
                    timer.startTime = t.startTime;
                    timer.endTime = t.endTime;
                    timer.recurringDays = t.recurringDays;
                    timer.channelId = t.channelId;
                    timer.channelTitle = t.channelTitle;
                    timer.description = t.description;
                    timers.removeProgrammeTimer(timer);
                }
            }
        }
    }

    Component {
        id: deleteMenu

        Menu {
            MenuItem {
                text: qsTr("Clear completed timers")
                onTriggered: timers.cleanupProgrammeTimers()
            }
            
            MenuItem {
                text: qsTr("Clear all timers")
                onTriggered: timers.clearProgrammeTimers()
            }
        }
    }
    
    Component {
        id: timerDialog
        
        ProgrammeTimerDialog {            
            onAccepted: timerModel.reload()
        }
    }

    contentItem.states: State {
        name: "Portrait"
        when: screen.currentOrientation == Qt.WA_Maemo5PortraitOrientation

        AnchorChanges {
            target: view
            anchors.right: parent.right
            anchors.bottom: buttonColumn.top
        }

        PropertyChanges {
            target: view
            anchors.rightMargin: 0
            anchors.bottomMargin: platformStyle.paddingMedium
            clip: true
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
    
    onStatusChanged: {
        if (status == DialogStatus.Open) {
            timerModel.reload();
            view.forceActiveFocus();
        }
    }
}
