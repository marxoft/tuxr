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

RemoteButton {
    id: root
    
    signal remove
    
    Image {
        id: image
        
        anchors {
            top: parent.top
            right: parent.right
            margins: -platformStyle.paddingLarge
        }
        width: 32
        height: 32
        source:  "images/close-thumbnail.png"
        asynchronous: true
        smooth: true
        visible: false
        
        MouseArea {
            anchors.fill: parent
            onClicked: root.remove()
        }
    }
    
    states: State {
        name: "edit"
        
        PropertyChanges {
            target: image
            visible: true
        }
    }
}
