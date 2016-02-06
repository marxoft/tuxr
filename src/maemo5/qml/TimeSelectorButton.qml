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

ValueButton {
    id: root
    
    property int hour: dateTime.currentHour()
    property int minute: dateTime.currentMinute()
    
    property bool _ready: false
    
    signal selected
    
    function _updateValueText() {
        valueText = Qt.formatTime(dateTime.time(hour, minute), TIME_FORMAT);
    }
    
    style: ValueButtonStyle {
        valueTextColor: platformStyle.activeTextColor
    }
        
    Component {
        id: selector
        
        TimePickSelector {
            hour: root.hour
            minute: root.minute
            valueTextFormat: TIME_FORMAT
            onSelected: {
                root._ready = false;
                root.hour = hour;
                root.minute = minute;
                root._ready = true;
                root.selected();
            }            
        }
    }
    
    onClicked: {
        if (!pickSelector) {
            pickSelector = selector.createObject(root);
            pickSelector.open();
        }
    }
    
    onHourChanged: if (_ready) _updateValueText();
    onMinuteChanged: if (_ready) _updateValueText();
    
    Component.onCompleted: {
        _updateValueText();
        _ready = true;
    }
}
