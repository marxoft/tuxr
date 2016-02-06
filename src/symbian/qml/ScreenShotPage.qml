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
import TuxR 1.0

MyPage {
    id: root
    
    property QtObject screenShot: instantiator.createScreenShot(settings.currentRemote, root)
    
    title: qsTr("Screenshot")
    tools: ToolBarLayout {
        BackToolButton {}
        
        MyToolButton {
            id: settingsButton
            
            iconSource: "toolbar-settings"
            toolTip: qsTr("Settings")
            onClicked: loader.open(imageTypeDialog)
        }
        
        MyToolButton {
            id: reloadButton
            
            iconSource: "toolbar-refresh"
            toolTip: qsTr("Reload")
            onClicked: screenShot.reload(settings.screenShotImageType)
        }
    }
    
    Pixmap {
        id: image
                
        width: parent.width - platformStyle.paddingLarge * 2
        height: Math.floor(Math.max(16, width * 9) / 16)
        anchors.centerIn: parent
        smooth: true
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
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: imageTypeDialog
        
        ValueDialog {
            focusItem: root
            titleText: qsTr("Image type")
            model: ScreenShotImageTypeModel {}
            value: settings.screenShotImageType
            onAccepted: settings.screenShotImageType = value
        }
    }
    
    states: State {
        name: "landscape"
        when: !appWindow.inPortrait
        
        PropertyChanges {
            target: image
            width: Math.floor(Math.max(9, height * 16) / 9)
            height: parent.height - platformStyle.paddingLarge * 2
        }
    }
    
    Component.onCompleted: screenShot.reload(settings.screenShotImageType)
}
