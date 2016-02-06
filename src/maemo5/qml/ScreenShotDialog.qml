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
    height: 360
    
    Pixmap {
        id: image
        
        anchors {
            horizontalCenter: imageTypeButton.horizontalCenter
            top: parent.top
            bottom: imageTypeButton.top
            bottomMargin: platformStyle.paddingMedium
        }
        width: Math.floor(Math.max(9, height * 16) / 9)
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
    
    onStatusChanged: if (status == DialogStatus.Open) screenShot.reload(settings.screenShotImageType);
}
