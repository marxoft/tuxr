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

MyPage {
    id: root
    
    title: loader.sourceComponent == remoteView ? loader.item.currentTab.title : "TuxR"
    showProgressIndicator: (loader.sourceComponent == remoteView)
                           && (loader.item.currentTab.showProgressIndicator)
    tools: (loader.sourceComponent == remoteView) && (loader.item.currentTab.tools)
           ? loader.item.currentTab.tools : ownTools
    onToolsChanged: if (status == PageStatus.Active) appWindow.pageStack.toolBar.setTools(tools, "set");

    ToolBarLayout {
        id: ownTools

        visible: false
        
        MyToolButton {
            anchors.right: parent.right
            iconSource: "toolbar-view-menu"
            toolTip: qsTr("Options")
            onClicked: appMenu.open()
        }
    }
    
    Loader {
        id: loader
        
        anchors.fill: parent
        sourceComponent: placeholder
    }
    
    Component {
        id: remoteView
        
        TabView {
            id: view
            
            anchors.fill: parent
            visible: settings.currentRemote != ""
            
            RemoteTab {
                id: remoteTab
                
                width: view.width
                height: view.height
                title: qsTr("Remote")
            }
            
            TabLoader {
                id: serviceTab
                
                width: view.width
                height: view.height
                title: qsTr("Zap")
                tab: Component {
                    ServiceTab {}
                }
            }
            
            TabLoader {
                id: timerTab
                
                width: view.width
                height: view.height
                title: qsTr("Timers")
                tab: Component {
                    ProgrammeTimerTab {}
                }
            }
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
                    margins: platformStyle.paddingLarge
                }
                spacing: platformStyle.paddingLarge
                
                Label {
                    id: label
                    
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    color: platformStyle.colorNormalMid
                    font.bold: true
                    font.pixelSize: 32
                    text: qsTr("No remote loaded")
                }
                
                MyButton {
                    x: Math.floor((parent.width - width) / 2)
                    text: qsTr("Add remote")
                    onClicked: appWindow.pageStack.push(Qt.resolvedUrl("RemotePage.qml"))
                }
            }
        }
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
    
    Component.onCompleted: if (settings.currentRemote) loader.sourceComponent = remoteView;
}
