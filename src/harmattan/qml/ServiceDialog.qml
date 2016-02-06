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

MySheet {
    id: root
    
    property variant service
    
    acceptButtonText: (service) && (service.serviceType == Service.Channel) ? qsTr("Done") : ""
    rejectButtonText: qsTr("Cancel")
    content: Item {
        anchors.fill: parent
        
        ToolBarLayout {
            id: tools
            
            height: 64
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            
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
        }
        
        ListView {
            id: view
            
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                bottom: tools.top
            }
            clip: true
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
                        
                        root.showProgressIndicator = false;
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
                            service = serviceModel.itemData(index, dataModel.rootIndex);
                        }
                    }
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
    }
    
    Component.onCompleted: serviceModel.reload(dataModel.rootIndex)
}
