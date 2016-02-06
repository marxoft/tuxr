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
    
    property int year: dateTime.currentYear()
    property int month: dateTime.currentMonth()
    property int day: dateTime.currentMonth()
    
    property bool _ready: false
    
    signal selected
    
    function _updateValueText() {
        valueText = Qt.formatDate(dateTime.date(year, month, day), DATE_FORMAT);
    }
    
    style: ValueButtonStyle {
        valueTextColor: platformStyle.activeTextColor
    }
        
    Component {
        id: selector
        
        DatePickSelector {
            year: root.year
            month: root.month
            day: root.day
            valueTextFormat: DATE_FORMAT
            onSelected: {
                root._ready = false;
                root.year = year;
                root.month = month;
                root.day = day;
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
    
    onYearChanged: if (_ready) _updateValueText();
    onMonthChanged: if (_ready) _updateValueText();
    onDayChanged: if (_ready) _updateValueText();
    
    Component.onCompleted: {
        _updateValueText();
        _ready = true;
    }
}
