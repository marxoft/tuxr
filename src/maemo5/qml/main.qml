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

Window {
    id: appWindow
    
    title: "TuxR"
    visible: true
    menuBar: MenuBar {
        id: appMenu
        
        filters: [
            MenuItem {
                id: remoteFilter
                
                checkable: true
                checked: true
                text: qsTr("Remote")
                enabled: loader.sourceComponent == remoteView
                onTriggered: loader.item.currentIndex = 0
            },
            
            MenuItem {
                id: serviceFilter
                
                checkable: true
                text: qsTr("Zap")
                enabled: loader.sourceComponent == remoteView
                onTriggered: loader.item.currentIndex = 1
            }
        ]
        
        MenuItem {
            action: screenShotAction
        }
        
        MenuItem {
            action: timerAction
        }
        
        MenuItem {
            action: remotesAction
        }
        
        MenuItem {
            action: aboutAction
        }
    }
    
    Action {
        id: leftAction
        
        autoRepeat: false
        shortcut: qsTr("Left")
        enabled: loader.sourceComponent == remoteView
        onTriggered: {
            switch (loader.item.currentIndex) {
            case 0:
                serviceFilter.trigger();
                break;
            default:
                remoteFilter.trigger();
                break;
            }
        }
    }
    
    Action {
        id: rightAction
        
        autoRepeat: false
        shortcut: qsTr("Right")
        enabled: loader.sourceComponent == remoteView
        onTriggered: {
            switch (loader.item.currentIndex) {
            case 0:
                serviceFilter.trigger();
                break;
            default:
                remoteFilter.trigger();
                break;
            }
        }
    }
    
    Action {
        id: screenShotAction
        
        autoRepeat: false
        shortcut: qsTr("Ctrl+P")
        text: qsTr("Screenshot")
        enabled: loader.sourceComponent == remoteView
        onTriggered: popupLoader.open(screenShotDialog, appWindow)
    }
    
    Action {
        id: timerAction
        
        autoRepeat: false
        shortcut: qsTr("Ctrl+T")
        text: qsTr("Timers")
        enabled: loader.sourceComponent == remoteView
        onTriggered: popupLoader.open(timerDialog, appWindow)
    }
    
    Action {
        id: remotesAction
        
        autoRepeat: false
        shortcut: qsTr("Ctrl+R")
        text: qsTr("Remotes")
        onTriggered: popupLoader.open(remoteDialog, appWindow)
    }
    
    Action {
        id: aboutAction
        
        autoRepeat: false
        shortcut: qsTr("Ctrl+H")
        text: qsTr("About")
        onTriggered: popupLoader.open(aboutDialog, appWindow)
    }
    
    InformationBox {
        id: infoBanner
        
        function showMessage(message) {
            infoLabel.text = message;
            open();
        }
        
        height: Math.max(30, infoLabel.height + platformStyle.paddingLarge)
        
        Label {
            id: infoLabel
            
            anchors {
                fill: parent
                leftMargin: platformStyle.paddingLarge
                rightMargin: platformStyle.paddingLarge
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: platformStyle.reversedTextColor
            wrapMode: Text.WordWrap
        }
    }
    
    Loader {
        id: loader
        
        anchors.fill: parent
    }
    
    Component {
        id: remoteView
        
        TabView {
            id: tabView
            
            anchors.fill: parent
            tabsVisible: false
            frameVisible: false
                        
            RemoteTab {
                id: remoteTab
                
                onShowProgressIndicatorChanged: appWindow.showProgressIndicator = (showProgressIndicator)
                                                || (serviceTab.showProgressIndicator)
            }
            
            ServiceTab {
                id: serviceTab
                
                onShowProgressIndicatorChanged: appWindow.showProgressIndicator = (showProgressIndicator)
                                                || (remoteTab.showProgressIndicator)
            }
            
            onCurrentIndexChanged: appWindow.title = "TuxR - " + getTab(currentIndex).title
            Component.onCompleted: appWindow.title = "TuxR - " + getTab(currentIndex).title
        }
    }
    
    Component {
        id: placeholder
        
        Item {
            anchors.fill: parent
            
            Column {
                id: column
                
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: platformStyle.paddingMedium
                }
                spacing: platformStyle.paddingMedium
                
                Label {
                    id: label
                    
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    color: platformStyle.disabledTextColor
                    font.pointSize: platformStyle.fontSizeLarge
                    text: qsTr("No remote loaded")
                }
                
                Button {
                    x: Math.floor((parent.width - width) / 2)
                    text: qsTr("Add remote")
                    onClicked: popupLoader.open(remoteDialog, appWindow)
                }
            }
            
            Component.onCompleted: appWindow.title = "TuxR"
        }
    }
    
    PopupLoader {
        id: popupLoader
    }
    
    Component {
        id: screenShotDialog
        
        ScreenShotDialog {}
    }
    
    Component {
        id: timerDialog
        
        ProgrammeTimersDialog {}
    }
    
    Component {
        id: remoteDialog
        
        RemotesDialog {}
    }
    
    Component {
        id: aboutDialog
        
        AboutDialog {}
    }
    
    Connections {
        target: settings
        onCurrentRemoteChanged: {
            if (loader.sourceComponent == remoteView) {
                loader.sourceComponent = undefined;
            }
            
            if (settings.currentRemote) {
                loader.sourceComponent = remoteView;
            }
            else {
                loader.sourceComponent = placeholder;
            }
        }
    }
    
    Component.onCompleted: loader.sourceComponent = (settings.currentRemote ? remoteView : placeholder);
}
