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

AbstractPickSelector {
    id: root
    
    property variant service
    
    title: qsTr("Channel")
    height: 360
    
    ListView {
        id: view
        
        property string searchText
        
        anchors {
            left: parent.left
            right: buttonColumn.left
            rightMargin: platformStyle.paddingMedium
            top: parent.top
            bottom: parent.bottom
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
                    
                    root.showProgressIndicator = false;
                    reloadButton.enabled = true;
                    label.visible = (services.count == 0);
                }
            }
            delegate: ServiceDelegate {
                style: ListItemStyle {
                    background: "image://theme/TouchListBackground"
                                 + ((service) && (service.id == id) ? "Pressed" : "Normal")
                }
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
        
        Keys.onPressed: {
            if ((!event.isAutoRepeat) && (event.text)) {
                searchText += event.text;
                searchTimer.restart();
                event.accepted = true;
            }
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
    
    DialogButtonStyle {
        id: dialogButtonStyle
    }
    
    Column {
        id: buttonColumn
        
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        spacing: platformStyle.paddingMedium
        
        Button {
            id: homeButton
            
            text: qsTr("Go home")
            style: dialogButtonStyle
            activeFocusOnPress: false
            onClicked: dataModel.rootIndex = undefined
        }
        
        Button {
            id: upButton
            
            text: qsTr("Go up")
            style: dialogButtonStyle
            activeFocusOnPress: false
            onClicked: dataModel.rootIndex = dataModel.parentModelIndex()
        }
        
        Button {
            id: reloadButton
            
            text: qsTr("Reload")
            style: dialogButtonStyle
            activeFocusOnPress: false
            onClicked: serviceModel.reload(dataModel.rootIndex)
        }
    
        Button {
            id: button

            text: qsTr("Done")
            style: dialogButtonStyle
            activeFocusOnPress: false
            enabled: (service) && (service.serviceType == Service.Channel) ? true : false
            onClicked: {
                root.selected(service.title);
                root.accept();
            }
        }
    }
    
    onStatusChanged: {
        if (status == DialogStatus.Open) {
            view.forceActiveFocus();
            
            if (view.count == 0) {
                serviceModel.reload(dataModel.rootIndex);
            }
        }
    }
}
