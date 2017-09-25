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
    
    title: timerId ? qsTr("Edit timer") : qsTr("Add timer")
    height: Math.min(360, column.height + platformStyle.paddingMedium)
    
    Flickable {
        id: flickable
        
        anchors {
            left: parent.left
            right: button.left
            rightMargin: platformStyle.paddingMedium
            top: parent.top
            bottom: parent.bottom
        }
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        contentHeight: column.height
        
        Column {
            id: column
            
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            spacing: platformStyle.paddingMedium
            
            ListSelectorButton {
                id: eventTypeButton
                
                width: parent.width
                text: qsTr("Event type")
                model: ProgrammeTimerEventTypeModel {
                    id: eventTypeModel
                }
                value: newTimer.eventType
                onSelected: newTimer.eventType = value
            }
            
            ListSelectorButton {
                id: actionButton
                
                width: parent.width
                text: qsTr("Action")
                model: ProgrammeTimerActionModel {
                    id: actionModel
                }
                value: newTimer.action
                onSelected: newTimer.action = value
            }
            
            ListSelectorButton {
                id: afterActionButton
                
                width: parent.width
                text: qsTr("After event")
                model: ProgrammeTimerAfterActionModel {
                    id: afterActionModel
                }
                value: newTimer.afterAction
                onSelected: newTimer.afterAction = value
            }
                        
            DateSelectorButton {
                id: startDateButton
                
                width: parent.width
                text: qsTr("Start date")
                year: dateTime.year(newTimer.startTime)
                month: dateTime.month(newTimer.startTime)
                day: dateTime.day(newTimer.startTime)
                enabled: newTimer.eventType == ProgrammeTimer.OnetimeEvent
                onSelected: newTimer.startTime = dateTime.dateTime(year, month, day,
                            dateTime.hour(newTimer.startTime), dateTime.minute(newTimer.startTime))
            }
            
            TimeSelectorButton {
                id: startTimeButton
                
                width: parent.width
                text: qsTr("Start time")
                hour: dateTime.hour(newTimer.startTime)
                minute: dateTime.minute(newTimer.startTime)
                onSelected: newTimer.startTime = dateTime.dateTime(dateTime.year(newTimer.startTime),
                            dateTime.month(newTimer.startTime), dateTime.day(newTimer.startTime),
                            hour, minute)
            }
            
            DateSelectorButton {
                id: endDateButton
                
                width: parent.width
                text: qsTr("End date")
                year: dateTime.year(newTimer.endTime)
                month: dateTime.month(newTimer.endTime)
                day: dateTime.day(newTimer.endTime)
                enabled: newTimer.eventType == ProgrammeTimer.OnetimeEvent
                onSelected: newTimer.endTime = dateTime.dateTime(year, month, day,
                            dateTime.hour(newTimer.endTime), dateTime.minute(newTimer.endTime))
            }
            
            TimeSelectorButton {
                id: endTimeButton
                
                width: parent.width
                text: qsTr("End time")
                hour: dateTime.hour(newTimer.endTime)
                minute: dateTime.minute(newTimer.endTime)
                onSelected: newTimer.endTime = dateTime.dateTime(dateTime.year(newTimer.endTime),
                            dateTime.month(newTimer.endTime), dateTime.day(newTimer.endTime),
                            hour, minute)
            }
            
            MultiListSelectorButton {
                id: recurringDaysButton
                
                width: parent.width
                text: qsTr("Recurring days")
                enabled: newTimer.eventType == ProgrammeTimer.RecurringEvent
                model: ProgrammeTimerRecurringDaysModel {
                    id: recurringDaysModel
                }
                onSelected: {
                    var days = 0;
                    
                    for (var i = 0; i < values.length; i++) {
                        days |= values[i];
                    }
                    
                    newTimer.recurringDays = days;
                }
            }
            
            SelectorButton {
                id: channelButton
                
                width: parent.width
                text: qsTr("Channel")
                valueText: newTimer.channelTitle ? newTimer.channelTitle : qsTr("None chosen")
                selector: ServicePickSelector {
                    id: serviceSelector
                    
                    currentValueText: newTimer.channelTitle ? newTimer.channelTitle : qsTr("None chosen")
                    onSelected: {
                        newTimer.channelId = service.id;
                        newTimer.channelTitle = service.title;
                    }
                }
            }
            
            Label {
                width: parent.width
                text: qsTr("Description")
            }
            
            TextField {
                id: descriptionField
                
                width: parent.width
                text: oldTimer.description
                onTextChanged: newTimer.description = text
            }
        }
    }
    
    Button {
        id: button
        
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        style: DialogButtonStyle {}
        text: qsTr("Done")
        enabled: (newTimer.channelId) && (newTimer.channelTitle)
        onClicked: timerId ? timers.updateProgrammeTimer(oldTimer, newTimer)
                           : timers.addProgrammeTimer(newTimer)
    }
    
    ProgrammeTimer {
        id: oldTimer
        
        onRecurringDaysChanged: {
            var days = [];
            
            for (var i = 0; i < recurringDaysModel.count; i++) {
                var day = recurringDaysModel.data(i, "value");
                
                if (recurringDays & day) {
                    days.push(day);
                }
            }
            
            recurringDaysButton.values = days;
        }
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

    contentItem.states: State {
        name: "Portrait"
        when: screen.currentOrientation == Qt.WA_Maemo5PortraitOrientation

        AnchorChanges {
            target: flickable
            anchors.right: parent.right
            anchors.bottom: button.top
        }

        PropertyChanges {
            target: flickable
            anchors.rightMargin: 0
            anchors.bottomMargin: platformStyle.paddingMedium
            clip: true
        }

        PropertyChanges {
            target: button
            width: parent.width
        }

        PropertyChanges {
            target: root
            height: Math.min(680, column.height + button.height + platformStyle.paddingMedium * 2)
        }
    }
}
