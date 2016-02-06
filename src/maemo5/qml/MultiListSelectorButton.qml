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
    
    property variant values: []
    property variant model
    
    property bool _ready: false
    
    signal selected
    
    function _updateValueText() {
        if ((!model) || (values.length == 0)) {
            valueText = qsTr("None chosen");
            return;
        }
        
        var vt = [];
        
        for (var i = 0; i < values.length; i++) {
            vt.push(model.data(model.match(0, "value", values[i]), "name"));
        }
        
        valueText = vt.join(", ");
    }
    
    style: ValueButtonStyle {
        valueTextColor: platformStyle.activeTextColor
    }
    
    Component {
        id: selector
        
        MultiListPickSelector {
            model: root.model
            textRole: "name"
            onSelected: {
                var v = [];
                
                for (var i = 0; i < currentIndexes.length; i++) {
                    v.push(model.data(currentIndexes[i], "value"));
                }
                
                root._ready = false;
                root.values = v;
                root._ready = true;
                root.selected();
            }
            
            onStatusChanged: {
                if (status == DialogStatus.Opening) {
                    var idxs = [];
                    
                    for (var i = 0; i < root.values.length; i++) {
                        idxs.push(model.match(0, "value", root.values[i]));
                    }
                    
                    currentIndexes = idxs;
                }
            }
        }
    }
    
    onClicked: {
        if (!pickSelector) {
            pickSelector = selector.createObject(root);
            pickSelector.open();
        }
    }
    
    onModelChanged: if (_ready) _updateValueText();
    onValuesChanged: if (_ready) _updateValueText();
    
    Component.onCompleted: {
        _updateValueText();
        _ready = true;
    }
}
