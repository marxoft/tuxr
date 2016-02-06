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

Tab {
    id: root
    
    property QtObject remote: instantiator.createRemote(settings.currentRemote, root)
        
    title: qsTr("Remote")
    tools: ToolBarLayout {
        MyToolButton {
            iconSource: "images/gallery.png"
            toolTip: qsTr("Screenshot")
            onClicked: appWindow.pageStack.push(Qt.resolvedUrl("ScreenShotPage.qml"))
        }
        
        MyToolButton {
            iconSource: "toolbar-add"
            toolTip: qsTr("Add macro")
            onClicked: loader.open(macroDialog)
        }
        
        MyToolButton {
            iconSource: "toolbar-view-menu"
            toolTip: qsTr("Options")
            onClicked: appMenu.open()
        }
    }
    
    KeyNavFlickable {
        id: flickable
        
        anchors.fill: parent
        contentHeight: container.height
        
        MouseArea {
            id: container
                        
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            height: numberPad.height + dPad.height + colorPad.height + macroLabel.height
                    + macroPad.height + platformStyle.paddingLarge * 10
        
            Grid {
                id: numberPad
                
                property int buttonWidth: Math.floor((width - spacing * (columns - 1)) / columns)
                
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    margins: platformStyle.paddingLarge
                }
                columns: 3
                spacing: platformStyle.paddingLarge
                
                RemoteSpacer {
                    width: numberPad.buttonWidth
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Power)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Power))
                    onClicked: remote.sendKey(Remote.Key_Power)
                }
                
                RemoteSpacer {
                    width: numberPad.buttonWidth
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_1)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_1))
                    onClicked: remote.sendKey(Remote.Key_1)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_2)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_2))
                    onClicked: remote.sendKey(Remote.Key_2)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_3)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_3))
                    onClicked: remote.sendKey(Remote.Key_3)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_4)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_4))
                    onClicked: remote.sendKey(Remote.Key_4)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_5)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_5))
                    onClicked: remote.sendKey(Remote.Key_5)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_6)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_6))
                    onClicked: remote.sendKey(Remote.Key_6)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_7)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_7))
                    onClicked: remote.sendKey(Remote.Key_7)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_8)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_8))
                    onClicked: remote.sendKey(Remote.Key_8)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_9)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_9))
                    onClicked: remote.sendKey(Remote.Key_9)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Back)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Back))
                    onClicked: remote.sendKey(Remote.Key_Back)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_0)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_0))
                    onClicked: remote.sendKey(Remote.Key_0)
                }
                
                RemoteButton {
                    width: numberPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Forward)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Forward))
                    onClicked: remote.sendKey(Remote.Key_Forward)
                }
            }
            
            Grid {
                id: dPad
                
                property int buttonWidth: Math.floor((width - spacing * (columns - 1)) / columns)
                
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingLarge
                    right: parent.right
                    rightMargin: platformStyle.paddingLarge
                    top: numberPad.bottom
                    topMargin: platformStyle.paddingLarge * 2
                }
                columns: 3
                spacing: platformStyle.paddingLarge
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_VolumeUp)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_VolumeUp))
                    onClicked: remote.sendKey(Remote.Key_VolumeUp)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_VolumeMute)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_VolumeMute))
                    onClicked: remote.sendKey(Remote.Key_VolumeMute)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_BouquetUp)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_BouquetUp))
                    onClicked: remote.sendKey(Remote.Key_BouquetUp)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_VolumeDown)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_VolumeDown))
                    onClicked: remote.sendKey(Remote.Key_VolumeDown)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Exit)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Exit))
                    onClicked: remote.sendKey(Remote.Key_Exit)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_BouquetDown)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_BouquetDown))
                    onClicked: remote.sendKey(Remote.Key_BouquetDown)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Info)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Info))
                    onClicked: remote.sendKey(Remote.Key_Info)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Up)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Up))
                    onClicked: remote.sendKey(Remote.Key_Up)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Menu)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Menu))
                    onClicked: remote.sendKey(Remote.Key_Menu)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Left)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Left))
                    onClicked: remote.sendKey(Remote.Key_Left)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Enter)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Enter))
                    onClicked: remote.sendKey(Remote.Key_Enter)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Right)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Right))
                    onClicked: remote.sendKey(Remote.Key_Right)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Audio)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Audio))
                    onClicked: remote.sendKey(Remote.Key_Audio)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Down)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Down))
                    onClicked: remote.sendKey(Remote.Key_Down)
                }
                
                RemoteButton {
                    width: dPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Video)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Video))
                    onClicked: remote.sendKey(Remote.Key_Video)
                }
            }
            
            Grid {
                id: colorPad
                
                property int buttonWidth: Math.floor((width - spacing * (columns - 1)) / columns)
                
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingLarge
                    right: parent.right
                    rightMargin: platformStyle.paddingLarge
                    top: dPad.bottom
                    topMargin: platformStyle.paddingLarge * 2
                }
                columns: 4
                spacing: platformStyle.paddingLarge
                
                RemoteButton {
                    width: colorPad.buttonWidth
                    iconSource: "images/red.png"
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Red))
                    onClicked: remote.sendKey(Remote.Key_Red)
                }
                
                RemoteButton {
                    width: colorPad.buttonWidth
                    iconSource: "images/green.png"
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Green))
                    onClicked: remote.sendKey(Remote.Key_Green)
                }
                
                RemoteButton {
                    width: colorPad.buttonWidth
                    iconSource: "images/yellow.png"
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Yellow))
                    onClicked: remote.sendKey(Remote.Key_Yellow)
                }
                
                RemoteButton {
                    width: colorPad.buttonWidth
                    iconSource: "images/blue.png"
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Blue))
                    onClicked: remote.sendKey(Remote.Key_Blue)
                }
                
                RemoteButton {
                    width: colorPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Tv)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Tv))
                    onClicked: remote.sendKey(Remote.Key_Tv)
                }
                
                RemoteButton {
                    width: colorPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Radio)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Radio))
                    onClicked: remote.sendKey(Remote.Key_Radio)
                }
                
                RemoteButton {
                    width: colorPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Text)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Text))
                    onClicked: remote.sendKey(Remote.Key_Text)
                }
                
                RemoteButton {
                    width: colorPad.buttonWidth
                    text: remoteKeys.text(Remote.Key_Help)
                    enabled: (remote) && (remote.keyIsEnabled(Remote.Key_Help))
                    onClicked: remote.sendKey(Remote.Key_Help)
                }
            }
            
            SeparatorLabel {
                id: macroLabel
                
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingLarge
                    right: parent.right
                    rightMargin: platformStyle.paddingLarge
                    top: colorPad.bottom
                    topMargin: platformStyle.paddingLarge * 2
                }
                text: qsTr("Macros")
            }
            
            Grid {
                id: macroPad
                
                property int buttonWidth: Math.floor((width - spacing * (columns - 1)) / columns)
                                
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingLarge
                    right: parent.right
                    rightMargin: platformStyle.paddingLarge
                    top: macroLabel.bottom
                    topMargin: platformStyle.paddingLarge * 2
                }
                columns: 4
                spacing: platformStyle.paddingLarge
                
                Repeater {
                    id: macroRepeater
                    
                    property int currentIndex: -1
                    
                    model: RemoteMacroModel {
                        id: macroModel
                        
                        Component.onCompleted: reload(settings.currentRemote)
                    }
                    
                    RemoteMacroButton {
                        z: index * -1
                        width: macroPad.buttonWidth
                        state: container.state
                        text: name
                        onRemove: if (macroModel.remove(index)) infoBanner.showMessage(qsTr("Macro removed"));
                        onClicked: {
                            if (state == "edit") {
                                macroRepeater.currentIndex = index;
                                var dialog = loader.load(macroDialog);
                                dialog.state = "edit";
                                dialog.text = name;
                                dialog.keys = value;
                                dialog.open();
                            }
                            else {
                                remote.sendKeys(value);
                            }
                        }
                    }
                }
            }
            
            states: State {
                name: "edit"
                
                PropertyChanges {
                    target: numberPad
                    z: -1
                    enabled: false
                }
                
                PropertyChanges {
                    target: dPad
                    z: -1
                    enabled: false
                }
                
                PropertyChanges {
                    target: colorPad
                    z: -1
                    enabled: false
                }
            }
            
            onClicked: state = ""
            onPressAndHold: if (macroModel.count > 0) state = "edit";
        }
    }
    
    ScrollDecorator {
        flickableItem: flickable
    }
    
    Connections {
        target: remote
        onStatusChanged: {
            switch (remote.status) {
            case Request.Active:
                root.showProgressIndicator = true;
                return;
            case Request.Error:
                infoBanner.showMessage(remote.errorString);
                break;
            default:
                break;
            }
            
            root.showProgressIndicator = false;
        }
    }
    
    PopupLoader {
        id: loader
    }
    
    Component {
        id: macroDialog
        
        RemoteMacroDialog {
            focusItem: flickable
            onAccepted: {
                if (state == "edit") {
                    if (macroModel.setItemData(macroRepeater.currentIndex, {name: text, value: keys})) {
                        infoBanner.showMessage(qsTr("Macro updated"));
                    }
                }
                else {
                    macroModel.append(text, keys);
                    infoBanner.showMessage(qsTr("Macro added"));
                }
            }
        }
    }

    states: State {
        name: "landscape"
        when: !appWindow.inPortrait
        
        AnchorChanges {
            target: numberPad
            anchors.right: parent.horizontalCenter
        }
        
        AnchorChanges {
            target: dPad
            anchors {
                left: parent.horizontalCenter
                top: parent.top
            }
        }
        
        PropertyChanges {
            target: dPad
            anchors.topMargin: platformStyle.paddingLarge
        }
        
        PropertyChanges {
            target: macroPad
            columns: 6
        }
        
        PropertyChanges {
            target: container
            height: dPad.height + colorPad.height + macroLabel.height + macroPad.height
                    + platformStyle.paddingLarge * 8
        }
    }    
}
