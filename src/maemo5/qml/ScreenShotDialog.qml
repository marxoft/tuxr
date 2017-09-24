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
    
    property QtObject screenShot: instantiator.createScreenShot(settings.currentRemote, root)
    
    title: qsTr("Screenshot")
    height: image.height + imageTypeButton.height + platformStyle.paddingMedium * 2
    
    Pixmap {
        id: image
        
        anchors {
            horizontalCenter: imageTypeButton.horizontalCenter
            bottom: imageTypeButton.top
            bottomMargin: platformStyle.paddingMedium
        }
        width: 440
        height: 248
        smooth: true
    }
    
    ListSelectorButton {
        id: imageTypeButton
        
        anchors {
            left: parent.left
            right: reloadButton.left
            rightMargin: platformStyle.paddingMedium
            bottom: parent.bottom
        }
        text: qsTr("Image type")
        model: ScreenShotImageTypeModel {
            id: imageTypeModel
        }
        value: settings.screenShotImageType
        onSelected: settings.screenShotImageType = value
    }
    
    Button {
        id: reloadButton
        
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        style: DialogButtonStyle {}
        text: qsTr("Reload")
        shortcut: qsTr("r")
        onClicked: screenShot.reload(settings.screenShotImageType)
    }
    
    Connections {
        target: screenShot
        onStatusChanged: {
            switch (screenShot.status) {
            case Request.Active: {
                root.showProgressIndicator = true;
                reloadButton.enabled = false;
                image.pixmap = undefined;
                return;
            }
            case Request.Ready:
                image.pixmap = screenShot.pixmap;
                break;
            case Request.Error:
                infoBanner.showMessage(screenShot.errorString);
                break;
            default:
                break;
            }
            
            root.showProgressIndicator = false;
            reloadButton.enabled = true;
        }
    }

    contentItem.states: State {
        name: "Portrait"
        when: screen.currentOrientation == Qt.WA_Maemo5PortraitOrientation

        AnchorChanges {
            target: imageTypeButton
            anchors.right: parent.right
            anchors.bottom: reloadButton.top
        }

        PropertyChanges {
            target: imageTypeButton
            anchors.rightMargin: 0
            anchors.bottomMargin: platformStyle.paddingMedium
        }

        PropertyChanges {
            target: reloadButton
            width: parent.width
        }

        PropertyChanges {
            target: root
            height: image.height + imageTypeButton.height + reloadButton.height + platformStyle.paddingMedium * 3
        }
    }
    
    onStatusChanged: if (status == DialogStatus.Open) screenShot.reload(settings.screenShotImageType);
}
