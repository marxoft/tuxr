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

Flickable {
    property ScrollBar scrollIndicator

    function moveUp() {
        if (scrollIndicator) {
            scrollIndicator.flash();
        }

        if (contentY > 100) {
            contentY -= 100;
        }
        else {
            contentY = 0;
        }
    }

    function moveDown() {
        if (scrollIndicator) {
            scrollIndicator.flash();
        }

        if (contentY < contentHeight - height - 100) {
            contentY += 100;
        }
        else {
            contentY = contentHeight - height;
        }
    }

    Keys.onUpPressed: moveUp()
    Keys.onDownPressed: moveDown()

    Component.onCompleted: forceActiveFocus()
    onVisibleChanged: if (visible) forceActiveFocus();

    Connections {
        target: inputContext
        onVisibleChanged: if ((!inputContext.visible) && (visible)) forceActiveFocus();
    }

    Behavior on contentY { NumberAnimation { duration: 250 } }
}
