/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 3, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1

MenuItem {
    id: root

    property string title: ""
    property string subTitle: ""

    Column {
        anchors {
            verticalCenter: parent.verticalCenter
            right: indicator.left
            rightMargin: platformStyle.paddingMedium
            left: parent.left
            leftMargin: platformStyle.paddingLarge
        }

        Loader {
            anchors.left: parent.left
            sourceComponent: title != "" ? titleText : undefined
            width: parent.width // elide requires explicit width
        }

        Loader {
            anchors.left: parent.left
            sourceComponent: subTitle != "" ? subTitleText : undefined
            width: parent.width // elide requires explicit width
        }
    }

    Image {
        id: indicator
        source: root.mode == "disabled" ? privateStyle.imagePath("qtg_graf_choice_list_indicator_disabled",
                                                                 root.platformInverted)
                                        : privateStyle.imagePath("qtg_graf_choice_list_indicator",
                                                                 root.platformInverted)
        sourceSize.width: platformStyle.graphicSizeSmall
        sourceSize.height: platformStyle.graphicSizeSmall
        anchors {
            right: parent.right
            rightMargin: platformStyle.paddingSmall
            verticalCenter: parent.verticalCenter
        }
    }

    Component {
        id: titleText
        MyListItemText {
            width: parent.width
            role: "SelectionTitle"
            text: root.title
            platformInverted: root.platformInverted
        }
    }

    Component {
        id: subTitleText
        MyListItemText {
            width: parent.width
            role: "SelectionSubTitle"
            text: root.subTitle
            platformInverted: root.platformInverted
        }
    }
}
