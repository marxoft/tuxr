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
    
    property variant value
    property variant model
    
    property bool _ready: false
    
    signal selected
    
    function _updateValueText() {
        if (model) {
            valueText = model.data(Math.max(0, model.match(0, "value", value)), "name");
        }
    }
    
    style: ValueButtonStyle {
        valueTextColor: platformStyle.activeTextColor
    }
    
    Component {
        id: selector
        
        ListPickSelector {
            model: root.model
            textRole: "name"
            onSelected: {
                root._ready = false;
                root.value = model.data(currentIndex, "value");
                root._ready = true;
                root.selected();
            }
            
            onStatusChanged: if (status == DialogStatus.Opening)
                             currentIndex = Math.max(0, model.match(0, "value", root.value));
            
        }
    }
    
    onClicked: {
        if (!pickSelector) {
            pickSelector = selector.createObject(root);
            pickSelector.open();
        }
    }
    
    onModelChanged: if (_ready) _updateValueText();
    onValueChanged: if (_ready) _updateValueText();
    
    Component.onCompleted: {
        _updateValueText();
        _ready = true;
    }
}
