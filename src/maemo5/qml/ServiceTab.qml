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

TuxrTab {
    id: root
        
    title: qsTr("Zap")
    
    Item {
        id: tab
        
        property QtObject remote: instantiator.createRemote(settings.currentRemote, tab)
        
        anchors.fill: parent
        
        ListView {
            id: view
            
            property string searchText

            function positionViewAtNextSection() {
                var i = Math.max(0, currentIndex);
                var s = serviceModel.data(i, dataModel.rootIndex, "section");
                ++i;

                while ((i < count - 1) && (serviceModel.data(i, dataModel.rootIndex, "section") == s)) {
                    ++i;
                }

                positionViewAtIndex(i, ListView.Beginning);
                currentIndex = i;
            }

            function positionViewAtPreviousSection() {
                var i = Math.max(0, currentIndex);
                var s = serviceModel.data(i, dataModel.rootIndex, "section");
                var ss;
                --i;

                while ((i > 0) && ((ss = serviceModel.data(i, dataModel.rootIndex, "section")) == s)) {
                    --i;
                }

                while ((i > 0) && (serviceModel.data(i - 1, dataModel.rootIndex, "section") == ss)) {
                    --i;
                }

                positionViewAtIndex(i, ListView.Beginning);
                currentIndex = i;
            }

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                bottom: toolbar.top
            }
            horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
            model: VisualDataModel {
                id: dataModel
                
                model: ServiceModel {
                    id: serviceModel
                    
                    services: instantiator.createServiceList(settings.currentRemote, serviceModel)
                    onStatusChanged: {
                        switch (status) {
                        case Request.Active: {
                            root.showProgressIndicator = true;
                            reloadButton.enabled = false;
                            label.visible = false;
                            return;
                        }
                        case Request.Error:
                            infoBanner.showMessage(errorString);
                            break;
                        default:
                            break;
                        }
                        
                        root.showProgressIndicator = (remote.status == Request.Active);
                        reloadButton.enabled = true;
                        label.visible = (services.count == 0);
                    }
                }
                delegate: ServiceDelegate {
                    onClicked: {
                        if (serviceType == Service.Bouquet) {
                            dataModel.rootIndex = dataModel.modelIndex(index);
                            
                            if (view.count == 0) {
                                serviceModel.reload(dataModel.rootIndex);
                            }
                        }
                        else {
                            remote.changeChannel(id);
                        }
                    }
                    onPressAndHold: popupManager.open(contextMenu, root)
                }
            }
            
            Keys.onPressed: {
                switch (event.key) {
                case Qt.Key_F7:
                    positionViewAtNextSection();
                    break;
                case Qt.Key_F8:
                    positionViewAtPreviousSection();
                    break;
                default:
                    if ((!event.isAutoRepeat) && (event.text)) {
                        searchText += event.text;
                        searchTimer.restart();
                        break;
                    }

                    return;
                }

                event.accepted = true;
            }
            
            onSearchTextChanged: {
                if (searchText) {
                    var index = serviceModel.match(currentIndex + 1, dataModel.rootIndex, "title", searchText);
                    
                    if (index != -1) {
                        navigationMode = NavigationMode.KeyNavigation;
                        currentIndex = index;
                    }
                }
            }
        }
        
        Timer {
            id: searchTimer
            
            interval: 1000
            onTriggered: view.searchText = ""
        }
        
        Label {
            id: label
            
            anchors {
                fill: view
                margins: platformStyle.paddingMedium
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            color: platformStyle.disabledTextColor
            font.pointSize: platformStyle.fontSizeLarge
            text: qsTr("No services")
        }
        
        ToolBar {
            id: toolbar
            
            anchors.bottom: parent.bottom
            alignment: Qt.AlignHCenter
            spacing: platformStyle.paddingMedium
            
            ToolButton {
                id: homeButton
                                    
                iconName: "general_presence_home"
                activeFocusOnPress: false
                onClicked: dataModel.rootIndex = undefined
            }
            
            ToolButton {
                id: upButton
                                    
                iconName: "rss_reader_move_up"
                activeFocusOnPress: false
                onClicked: dataModel.rootIndex = dataModel.parentModelIndex()
            }
            
            ToolButton {
                id: reloadButton
                                    
                iconName: "general_refresh"
                activeFocusOnPress: false
                onClicked: serviceModel.reload(dataModel.rootIndex)
            }
        }
        
        Connections {
            target: remote
            onStatusChanged: {
                switch (remote.status) {
                case Request.Active:
                    root.showProgressIndicator = true;
                    return;
                case Request.Ready:
                    infoBanner.showMessage(qsTr("Channel changed"));
                    break;
                case Request.Error:
                    infoBanner.showMessage(remote.errorString);
                    break;
                default:
                    break;
                }
                
                root.showProgressIndicator = (serviceModel.status == Request.Active);
            }
        }

        Component {
            id: contextMenu

            Menu {
                MenuItem {
                    text: qsTr("View")
                    onTriggered: {
                        if (serviceModel.data(view.currentIndex, dataModel.rootIndex, "serviceType")
                            == Service.Bouquet) {
                            dataModel.rootIndex = dataModel.modelIndex(view.currentIndex);
                            
                            if (view.count == 0) {
                                serviceModel.reload(dataModel.rootIndex);
                            }
                        }
                        else {
                            remote.changeChannel(serviceModel.data(view.currentIndex,
                            dataModel.rootIndex, "id"));
                        }
                    }
                }
                
                MenuItem {
                    text: qsTr("Show EPG")
                    onTriggered: {
                        var service = serviceModel.itemData(view.currentIndex, dataModel.rootIndex);
                        popupManager.open(programmeDialog, root, {serviceId: service.id, title: qsTr("EPG") + " - "
                            + service.title});
                    }
                }
            }
        }
        
        Component {
            id: programmeDialog
            
            ProgrammeDialog {}
        }
        
        Component.onCompleted: {
            serviceModel.reload(dataModel.rootIndex);
            view.forceActiveFocus();
        }
    }
}
