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
import com.nokia.meego 1.0
import TuxR 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Tab {
    id: root
    
    property QtObject remote: instantiator.createRemote(settings.currentRemote, root)
    
    title: qsTr("Zap")
    tools: ToolBarLayout {
        MyToolIcon {
            id: homeButton
            
            platformIconId: "toolbar-home"
            onClicked: dataModel.rootIndex = undefined
        }
        
        MyToolIcon {
            id: upButton
            
            platformIconId: "toolbar-up"
            onClicked: dataModel.rootIndex = dataModel.parentModelIndex()
        }
        
        MyToolIcon {
            id: reloadButton
            
            platformIconId: "toolbar-refresh"
            onClicked: serviceModel.reload(dataModel.rootIndex)
        }
        
        MyToolIcon {
            id: menuButton
            
            platformIconId: "toolbar-view-menu"
            onClicked: appMenu.open()
        }
    }
    
    ListView {
        id: view
        
        anchors.fill: parent
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
                onPressAndHold: contextMenu.open()
            }
        }
        section.delegate: SectionDelegate {
            text: section
        }
        section.property: "section"
    }
    
    MySectionScroller {
        listView: view
    }
    
    ContextMenu {
        id: contextMenu
        
        MenuLayout {
            MenuItem {
                text: qsTr("View")
                onClicked: {
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
                onClicked: {
                    var service = serviceModel.itemData(view.currentIndex, dataModel.rootIndex);
                    appWindow.pageStack.push(Qt.resolvedUrl("ProgrammePage.qml"),
                    {serviceId: service.id, title: qsTr("EPG") + " - " + service.title});
                }
            }
        }
    }
    
    Label {
        id: label
        
        anchors {
            fill: parent
            margins: UI.PADDING_DOUBLE
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: UI.COLOR_INVERTED_SECONDARY_FOREGROUND
        font.bold: true
        font.pixelSize: 40
        text: qsTr("No services")
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
    
    Component.onCompleted: serviceModel.reload(dataModel.rootIndex)
}
