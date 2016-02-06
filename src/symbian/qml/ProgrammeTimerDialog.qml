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
import com.nokia.extras 1.1
import TuxR 1.0

MySheet {
    id: root
    
    property alias timerId: oldTimer.id
    property alias eventType: oldTimer.eventType
    property alias action: oldTimer.action
    property alias afterAction: oldTimer.afterAction
    property alias startTime: oldTimer.startTime
    property alias endTime: oldTimer.endTime
    property alias recurringDays: oldTimer.recurringDays
    property alias channelId: oldTimer.channelId
    property alias channelTitle: oldTimer.channelTitle
    property alias description: oldTimer.description
    
    property QtObject timers: instantiator.createProgrammeTimers(settings.currentRemote, root)
    
    acceptButtonText: (newTimer.channelId) && (newTimer.channelTitle) ? qsTr("Done") : ""
    rejectButtonText: qsTr("Cancel")
    acceptWhenDone: false
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
                    topMargin: platformStyle.paddingLarge
                }
                spacing: platformStyle.paddingLarge
                
                Label {
                    x: platformStyle.paddingLarge
                    width: parent.width - platformStyle.paddingLarge * 2
                    font.pixelSize: platformStyle.fontSizeLarge
                    font.bold: true
                    text: timerId ? qsTr("Edit timer") :  qsTr("Add timer")
                    visible: !inputContext.visible
                }
                
                Rectangle {
                    x: platformStyle.paddingLarge
                    width: parent.width - platformStyle.paddingLarge * 2
                    height: 1
                    color: platformStyle.colorDisabledMid
                    visible: !inputContext.visible
                }
                
                ValueSelector {
                    id: eventTypeSelector
                    
                    width: parent.width
                    focusItem: flickable
                    title: qsTr("Event type")
                    model: ProgrammeTimerEventTypeModel {}
                    value: newTimer.eventType
                    visible: !inputContext.visible
                    onAccepted: newTimer.eventType = value
                }
                
                ValueSelector {
                    id: actionSelector
                    
                    width: parent.width
                    focusItem: flickable
                    title: qsTr("Action")
                    model: ProgrammeTimerActionModel {}
                    value: newTimer.action
                    visible: !inputContext.visible
                    onAccepted: newTimer.action = value
                }
                
                ValueSelector {
                    id: afterActionSelector
                    
                    width: parent.width
                    focusItem: flickable
                    title: qsTr("After event")
                    model: ProgrammeTimerAfterActionModel {}
                    value: newTimer.afterAction
                    visible: !inputContext.visible
                    onAccepted: newTimer.afterAction = value
                }
                
                ValueListItem {
                    id: startDateSelector
                    
                    width: parent.width
                    title: qsTr("Start date")
                    subTitle: Qt.formatDate(newTimer.startTime, DATE_FORMAT)
                    enabled: newTimer.eventType == ProgrammeTimer.OnetimeEvent
                    visible: !inputContext.visible
                    onClicked: loader.open(startDateDialog)
                }
                
                ValueListItem {
                    id: startTimeSelector
                    
                    width: parent.width
                    title: qsTr("Start time")
                    subTitle: Qt.formatTime(newTimer.startTime, TIME_FORMAT)
                    visible: !inputContext.visible
                    onClicked: loader.open(startTimeDialog)
                }
                
                ValueListItem {
                    id: endDateSelector
                    
                    width: parent.width
                    title: qsTr("End date")
                    subTitle: Qt.formatDate(newTimer.endTime, DATE_FORMAT)
                    enabled: newTimer.eventType == ProgrammeTimer.OnetimeEvent
                    visible: !inputContext.visible
                    onClicked: loader.open(endDateDialog)
                }
                
                ValueListItem {
                    id: endTimeSelector
                    
                    width: parent.width
                    title: qsTr("End time")
                    subTitle: Qt.formatTime(newTimer.endTime, TIME_FORMAT)
                    visible: !inputContext.visible
                    onClicked: loader.open(endTimeDialog)
                }
                
                Label {
                    x: platformStyle.paddingLarge
                    width: parent.width - platformStyle.paddingLarge * 2
                    text: qsTr("Recurring days")
                }
                
                Grid {
                    x: platformStyle.paddingLarge
                    width: parent.width - platformStyle.paddingLarge * 2
                    spacing: platformStyle.paddingLarge
                    columns: 2
                    enabled: newTimer.eventType == ProgrammeTimer.RecurringEvent
                    visible: !inputContext.visible
                    
                    Repeater {
                        model: ProgrammeTimerRecurringDaysModel {
                            id: recurringDaysModel
                        }
                        
                        MyCheckBox {
                            text: name
                            checked: newTimer.recurringDays & value
                            onCheckedChanged: checked ? newTimer.recurringDays |= value
                                                      : newTimer.recurringDays &= ~value
                        }
                    }
                }
                
                ValueListItem {
                    id: channelSelector
                    
                    width: parent.width
                    title: qsTr("Channel")
                    subTitle: newTimer.channelTitle ? newTimer.channelTitle : qsTr("None chosen")
                    visible: !inputContext.visible
                    onClicked: {
                        var dialog = serviceDialog.createObject(root);
                        dialog.open();
                    }
                }
                
                Label {
                    x: platformStyle.paddingLarge
                    width: parent.width - platformStyle.paddingLarge * 2
                    text: qsTr("Description")
                    visible: descriptionField.visible
                }
                
                MyTextField {
                    id: descriptionField
                    
                    x: platformStyle.paddingLarge
                    width: parent.width - platformStyle.paddingLarge * 2
                    text: oldTimer.description
                    visible: (!inputContext.visible) || (focus)
                    onTextChanged: newTimer.description = text
                    onAccepted: closeSoftwareInputPanel()
                }
            }
        }
        
        ScrollDecorator {
            flickableItem: flickable
        }
    }
    
    ProgrammeTimer {
        id: oldTimer
    }
    
    ProgrammeTimer {
        id: newTimer
        
        eventType: oldTimer.eventType
        action: oldTimer.action
        afterAction: oldTimer.afterAction
        startTime: oldTimer.startTime
        endTime: oldTimer.endTime
        recurringDays: oldTimer.recurringDays
        channelId: oldTimer.channelId
        channelTitle: oldTimer.channelTitle
        description: oldTimer.description
    }
    
    Connections {
        target: timers
        onStatusChanged: {
            switch (timers.status) {
            case Request.Active:
                root.showProgressIndicator = true;
                return;
            case Request.Ready: {
                infoBanner.showMessage(timerId ? qsTr("Timer updated") : qsTr("Timer added"));
                root.accept();
                break;
            }
            case Request.Error:
                infoBanner.showMessage(timers.errorString);
                break;
            default:
                break;
            }
            
            root.showProgressIndicator = false;
        }
    }
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: startDateDialog
        
        DatePickerDialog {
            titleText: qsTr("Start date")
            acceptButtonText: qsTr("Done")
            rejectButtonText: qsTr("Cancel")
            onClickedOutside: reject()
            onAccepted: newTimer.startTime = utils.dateTime(year, month, day, utils.hour(newTimer.startTime),
                                                            utils.minute(newTimer.startTime))
            onStatusChanged: if (status == DialogStatus.Closed) flickable.forceActiveFocus();
            Component.onCompleted: {
                day = utils.day(newTimer.startTime);
                month = utils.month(newTimer.startTime);
                year = utils.year(newTimer.startTime);
            }
        }
    }
    
    Component {
        id: startTimeDialog
        
        TimePickerDialog {
            titleText: qsTr("Start time")
            fields: DateTime.Hours | DateTime.Minutes
            acceptButtonText: qsTr("Done")
            rejectButtonText: qsTr("Cancel")
            onClickedOutside: reject()
            onAccepted: newTimer.startTime = utils.dateTime(utils.year(newTimer.startTime),
                                                            utils.month(newTimer.startTime),
                                                            utils.day(newTimer.startTime),
                                                            hour, minute);
            onStatusChanged: if (status == DialogStatus.Closed) flickable.forceActiveFocus();
            Component.onCompleted: {
                hour = utils.hour(newTimer.startTime);
                minute = utils.minute(newTimer.startTime);
            }
        }
    }
    
    Component {
        id: endDateDialog
        
        DatePickerDialog {
            titleText: qsTr("End date")
            acceptButtonText: qsTr("Done")
            rejectButtonText: qsTr("Cancel")
            onClickedOutside: reject()
            onAccepted: newTimer.endTime = utils.dateTime(year, month, day, utils.hour(newTimer.endTime),
                                                          utils.minute(newTimer.endTime))
            onStatusChanged: if (status == DialogStatus.Closed) flickable.forceActiveFocus();
            Component.onCompleted: {
                day = utils.day(newTimer.endTime);
                month = utils.month(newTimer.endTime);
                year = utils.year(newTimer.endTime);
            }
        }
    }
    
    Component {
        id: endTimeDialog
        
        TimePickerDialog {
            titleText: qsTr("End time")
            fields: DateTime.Hours | DateTime.Minutes
            acceptButtonText: qsTr("Done")
            rejectButtonText: qsTr("Cancel")
            onClickedOutside: reject()
            onAccepted: newTimer.endTime = utils.dateTime(utils.year(newTimer.endTime),
                                                          utils.month(newTimer.endTime),
                                                          utils.day(newTimer.endTime),
                                                          hour, minute);
            onStatusChanged: if (status == DialogStatus.Closed) flickable.forceActiveFocus();
            Component.onCompleted: {
                hour = utils.hour(newTimer.endTime);
                minute = utils.minute(newTimer.endTime);
            }
        }
    }
    
    Component {
        id: serviceDialog
        
        ServiceDialog {
            focusItem: flickable
            onAccepted: {
                newTimer.channelId = service.id;
                newTimer.channelTitle = service.title;
            }
        }
    }
    
    onDone: timerId ? timers.updateProgrammeTimer(oldTimer, newTimer)
                    : timers.addProgrammeTimer(newTimer)    
}
