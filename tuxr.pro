TEMPLATE = app
TARGET = tuxr

QT += network

#DEFINES += TUXR_DEBUG

INCLUDEPATH += \
    src/base \
    src/remotes \
    src/remotes/enigma1

HEADERS += \
    src/base/instantiator.h \
    src/base/programme.h \
    src/base/programmemodel.h \
    src/base/programmetimer.h \
    src/base/programmetimeractionmodel.h \
    src/base/programmetimerafteractionmodel.h \
    src/base/programmetimereventtypemodel.h \
    src/base/programmetimermodel.h \
    src/base/programmetimerrecurringdaysmodel.h \
    src/base/remoteconfigmodel.h \
    src/base/remoteconfigs.h \
    src/base/remotekeymodel.h \
    src/base/remotekeys.h \
    src/base/remotemacromodel.h \
    src/base/remotetypemodel.h \
    src/base/request.h \
    src/base/screenshotimagetypemodel.h \
    src/base/selectionmodel.h \
    src/base/service.h \
    src/base/servicemodel.h \
    src/base/tuxr.h \
    src/remotes/programmelist.h \
    src/remotes/programmetimerlist.h \
    src/remotes/programmetimers.h \
    src/remotes/remote.h \
    src/remotes/screenshot.h \
    src/remotes/servicelist.h \
    src/remotes/enigma1/enigma1programmelist.h \
    src/remotes/enigma1/enigma1programmetimerlist.h \
    src/remotes/enigma1/enigma1programmetimers.h \
    src/remotes/enigma1/enigma1remote.h \
    src/remotes/enigma1/enigma1screenshot.h \
    src/remotes/enigma1/enigma1servicelist.h

SOURCES += \
    src/base/instantiator.cpp \
    src/base/programme.cpp \
    src/base/programmemodel.cpp \
    src/base/programmetimer.cpp \
    src/base/programmetimermodel.cpp \
    src/base/remoteconfigmodel.cpp \
    src/base/remoteconfigs.cpp \
    src/base/remotemacromodel.cpp \
    src/base/request.cpp \
    src/base/selectionmodel.cpp \
    src/base/service.cpp \
    src/base/servicemodel.cpp \
    src/remotes/enigma1/enigma1programmelist.cpp \
    src/remotes/enigma1/enigma1programmetimerlist.cpp \
    src/remotes/enigma1/enigma1programmetimers.cpp \
    src/remotes/enigma1/enigma1remote.cpp \
    src/remotes/enigma1/enigma1screenshot.cpp \
    src/remotes/enigma1/enigma1servicelist.cpp

maemo5 {
    QT += declarative

    INCLUDEPATH += src/maemo5

    HEADERS += \
        src/maemo5/pixmapitem.h \
        src/maemo5/settings.h

    SOURCES += \
        src/maemo5/main.cpp \
        src/maemo5/pixmapitem.cpp \
        src/maemo5/settings.cpp
    
    qml.files = \
        src/maemo5/qml/AboutDialog.qml \
        src/maemo5/qml/DateSelectorButton.qml \
        src/maemo5/qml/ListSelectorButton.qml \
        src/maemo5/qml/main.qml \
        src/maemo5/qml/MultiListSelectorButton.qml \
        src/maemo5/qml/PopupLoader.qml \
        src/maemo5/qml/ProgrammeDelegate.qml \
        src/maemo5/qml/ProgrammeDetailsDialog.qml \
        src/maemo5/qml/ProgrammeDialog.qml \
        src/maemo5/qml/ProgrammeTimerDelegate.qml \
        src/maemo5/qml/ProgrammeTimerDialog.qml \
        src/maemo5/qml/ProgrammeTimersDialog.qml \
        src/maemo5/qml/RemoteButton.qml \
        src/maemo5/qml/RemoteDelegate.qml \
        src/maemo5/qml/RemoteDialog.qml \
        src/maemo5/qml/RemoteMacroButton.qml \
        src/maemo5/qml/RemoteMacroDialog.qml \
        src/maemo5/qml/RemotesDialog.qml \
        src/maemo5/qml/RemoteSpacer.qml \
        src/maemo5/qml/RemoteTab.qml \
        src/maemo5/qml/ScreenShotDialog.qml \
        src/maemo5/qml/SelectorButton.qml \
        src/maemo5/qml/ServiceDelegate.qml \
        src/maemo5/qml/ServicePickSelector.qml \
        src/maemo5/qml/ServiceTab.qml \
        src/maemo5/qml/TimeSelectorButton.qml \
        src/maemo5/qml/TuxrTab.qml
    
    qml.path = /opt/tuxr/qml
    
    desktop.files = desktop/maemo5/tuxr.desktop
    desktop.path = /usr/share/applications/hildon

    icon.files = desktop/maemo5/64/tuxr.png
    icon.path = /usr/share/icons/hicolor/64x64/apps
    
    target.path = /opt/tuxr/bin

    INSTALLS += \
        qml \
        desktop \
        icon

} else:contains(MEEGO_EDITION,harmattan) {
    CONFIG += qdeclarative-boostable
    
    QT += \
        declarative \
        opengl

    INCLUDEPATH += src/harmattan

    HEADERS += \
        src/harmattan/activecolormodel.h \
        src/harmattan/datetimeutils.h \
        src/harmattan/pixmapitem.h \
        src/harmattan/screenorientationmodel.h \
        src/harmattan/settings.h
    
    SOURCES += \
        src/harmattan/datetimeutils.cpp \
        src/harmattan/main.cpp \
        src/harmattan/pixmapitem.cpp \
        src/harmattan/settings.cpp
        
    qml.files = \
        src/harmattan/qml/AboutPage.qml \
        src/harmattan/qml/BackToolIcon.qml \
        src/harmattan/qml/LinkLabel.qml \
        src/harmattan/qml/ListItem.qml \
        src/harmattan/qml/main.qml \
        src/harmattan/qml/MainPage.qml \
        src/harmattan/qml/MyButton.qml \
        src/harmattan/qml/MyCheckBox.qml \
        src/harmattan/qml/MyInfoBanner.qml \
        src/harmattan/qml/MyPage.qml \
        src/harmattan/qml/MySectionScroller.qml \
        src/harmattan/qml/MySelectionDialog.qml \
        src/harmattan/qml/MySheet.qml \
        src/harmattan/qml/MySwitch.qml \
        src/harmattan/qml/MyTextField.qml \
        src/harmattan/qml/MyToolIcon.qml \
        src/harmattan/qml/PopupLoader.qml \
        src/harmattan/qml/ProgrammeDelegate.qml \
        src/harmattan/qml/ProgrammeDetailsPage.qml \
        src/harmattan/qml/ProgrammePage.qml \
        src/harmattan/qml/ProgrammeTimerDelegate.qml \
        src/harmattan/qml/ProgrammeTimerDialog.qml \
        src/harmattan/qml/ProgrammeTimerTab.qml \
        src/harmattan/qml/RemoteButton.qml \
        src/harmattan/qml/RemoteDelegate.qml \
        src/harmattan/qml/RemoteDialog.qml \
        src/harmattan/qml/RemoteMacroButton.qml \
        src/harmattan/qml/RemoteMacroDialog.qml \
        src/harmattan/qml/RemotePage.qml \
        src/harmattan/qml/RemoteSpacer.qml \
        src/harmattan/qml/RemoteTab.qml \
        src/harmattan/qml/ScreenShotPage.qml \
        src/harmattan/qml/SectionDelegate.qml \
        src/harmattan/qml/SectionScroller.js \
        src/harmattan/qml/SeparatorLabel.qml \
        src/harmattan/qml/ServiceDelegate.qml \
        src/harmattan/qml/ServiceDialog.qml \
        src/harmattan/qml/ServiceTab.qml \
        src/harmattan/qml/SettingsPage.qml \
        src/harmattan/qml/Tab.qml \
        src/harmattan/qml/TabLoader.qml \
        src/harmattan/qml/TabView.qml \
        src/harmattan/qml/TitleHeader.qml \
        src/harmattan/qml/ValueDialog.qml \
        src/harmattan/qml/ValueListItem.qml \
        src/harmattan/qml/ValueSelector.qml

    qml.path = /opt/tuxr/qml
    
    images.files = \
        src/harmattan/qml/images/blue.png \
        src/harmattan/qml/images/green.png \
        src/harmattan/qml/images/red.png \
        src/harmattan/qml/images/yellow.png
    
    images.path = /opt/tuxr/qml/images
    
    desktop.files = desktop/harmattan/tuxr.desktop
    desktop.path = /usr/share/applications

    icon.files = desktop/harmattan/80/tuxr.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    
    splash.files = \
        desktop/harmattan/splash/splash-landscape.png \
        desktop/harmattan/splash/splash-portrait.png
    
    splash.path = /opt/tuxr/splash
    
    target.path = /opt/tuxr/bin

    INSTALLS += \
        qml \
        images \
        desktop \
        icon \
        splash

} else:symbian|simulator {
    TARGET = tuxr_0xe72cbb8e
    TARGET.UID3 = 0xE72CBB8E
    TARGET.CAPABILITY += NetworkServices ReadUserData
    TARGET.EPOCHEAPSIZE = 0x20000 0x8000000
    TARGET.EPOCSTACKSIZE = 0x14000
    
    VERSION = 0.0.2
    ICON = desktop/symbian/tuxr.svg
    
    MMP_RULES += "DEBUGGABLE_UDEBONLY"

    CONFIG += qtcomponents
    
    QT += declarative

    INCLUDEPATH += src/symbian

    HEADERS += \
        src/symbian/datetimeutils.h \
        src/symbian/pixmapitem.h \
        src/symbian/screenorientationmodel.h \
        src/symbian/settings.h
    
    SOURCES += \
        src/symbian/datetimeutils.cpp \
        src/symbian/main.cpp \
        src/symbian/pixmapitem.cpp \
        src/symbian/settings.cpp
        
    qml.sources = \
        src/symbian/qml/AboutPage.qml \
        src/symbian/qml/AppWindow.qml \
        src/symbian/qml/BackToolButton.qml \
        src/symbian/qml/KeyNavFlickable.qml \
        src/symbian/qml/LinkLabel.qml \
        src/symbian/qml/main.qml \
        src/symbian/qml/MainPage.qml \
        src/symbian/qml/MyButton.qml \
        src/symbian/qml/MyCheckBox.qml \
        src/symbian/qml/MyContextMenu.qml \
        src/symbian/qml/MyDialog.qml \
        src/symbian/qml/MyFlickable.qml \
        src/symbian/qml/MyInfoBanner.qml \
        src/symbian/qml/MyListItem.qml \
        src/symbian/qml/MyListItemText.qml \
        src/symbian/qml/MyListView.qml \
        src/symbian/qml/MyMenu.qml \
        src/symbian/qml/MyPage.qml \
        src/symbian/qml/MyScrollBar.qml \
        src/symbian/qml/MySectionScroller.qml \
        src/symbian/qml/MySelectionDialog.qml \
        src/symbian/qml/MySheet.qml \
        src/symbian/qml/MySwitch.qml \
        src/symbian/qml/MyTextField.qml \
        src/symbian/qml/MyToolButton.qml \
        src/symbian/qml/PopupLoader.qml \
        src/symbian/qml/ProgrammeDelegate.qml \
        src/symbian/qml/ProgrammeDetailsPage.qml \
        src/symbian/qml/ProgrammePage.qml \
        src/symbian/qml/ProgrammeTimerDelegate.qml \
        src/symbian/qml/ProgrammeTimerDialog.qml \
        src/symbian/qml/ProgrammeTimerTab.qml \
        src/symbian/qml/RemoteButton.qml \
        src/symbian/qml/RemoteDelegate.qml \
        src/symbian/qml/RemoteDialog.qml \
        src/symbian/qml/RemoteMacroButton.qml \
        src/symbian/qml/RemoteMacroDialog.qml \
        src/symbian/qml/RemotePage.qml \
        src/symbian/qml/RemoteSpacer.qml \
        src/symbian/qml/RemoteTab.qml \
        src/symbian/qml/ScreenShotPage.qml \
        src/symbian/qml/SectionDelegate.qml \
        src/symbian/qml/SectionScroller.js \
        src/symbian/qml/SeparatorLabel.qml \
        src/symbian/qml/ServiceDelegate.qml \
        src/symbian/qml/ServiceDialog.qml \
        src/symbian/qml/ServiceTab.qml \
        src/symbian/qml/SettingsPage.qml \
        src/symbian/qml/Tab.qml \
        src/symbian/qml/TabLoader.qml \
        src/symbian/qml/TabView.qml \
        src/symbian/qml/TitleHeader.qml \
        src/symbian/qml/ValueDialog.qml \
        src/symbian/qml/ValueListItem.qml \
        src/symbian/qml/ValueSelector.qml

    qml.path = !:/Private/e72cbb8e/qml
    
    images.sources = \
        src/symbian/qml/images/blue.png \
        src/symbian/qml/images/clock.png \
        src/symbian/qml/images/close-thumbnail.png \
        src/symbian/qml/images/corner-left.png \
        src/symbian/qml/images/corner-right.png \
        src/symbian/qml/images/done.png \
        src/symbian/qml/images/gallery.png \
        src/symbian/qml/images/green.png \
        src/symbian/qml/images/home.png \
        src/symbian/qml/images/yellow.png \
        src/symbian/qml/images/tuxr.svg \
        src/symbian/qml/images/up.png \
        src/symbian/qml/images/yellow.png

    images.path = !:/Private/e72cbb8e/qml/images

    
    vendorinfo += "%{\"Stuart Howarth\"}" ":\"Stuart Howarth\""
    qtcomponentsdep = "; Default dependency to Qt Quick Components for Symbian library" \
        "(0x200346DE), 1, 1, 0, {\"Qt Quick components for Symbian\"}"

    tuxr_deployment.pkg_prerules += \
        vendorinfo \
        qtcomponentsdep

    DEPLOYMENT.display_name = TuxR
    DEPLOYMENT += \
        tuxr_deployment \
        qml \
        images

} else:unix {
    QT += \
        multimedia \
        multimediawidgets \
        widgets
    
    INCLUDEPATH += src/desktop
    
    HEADERS += \
        src/desktop/mainwindow.h \
        src/desktop/programmedialog.h \
        src/desktop/programmesdialog.h \
        src/desktop/programmetimerdialog.h \
        src/desktop/programmetimerview.h \
        src/desktop/programmeview.h \
        src/desktop/remotedialog.h \
        src/desktop/remotesdialog.h \
        src/desktop/remoteview.h \
        src/desktop/remotewidget.h \
        src/desktop/screenshotdialog.h \
        src/desktop/servicedialog.h \
        src/desktop/serviceview.h \
        src/desktop/settings.h \
        src/desktop/videoplayer.h

    SOURCES += \
        src/desktop/main.cpp \
        src/desktop/mainwindow.cpp \
        src/desktop/programmedialog.cpp \
        src/desktop/programmesdialog.cpp \
        src/desktop/programmetimerdialog.cpp \
        src/desktop/programmetimerview.cpp \
        src/desktop/programmeview.cpp \
        src/desktop/remotedialog.cpp \
        src/desktop/remotesdialog.cpp \
        src/desktop/remoteview.cpp \
        src/desktop/remotewidget.cpp \
        src/desktop/screenshotdialog.cpp \
        src/desktop/servicedialog.cpp \
        src/desktop/serviceview.cpp \
        src/desktop/settings.cpp \
        src/desktop/videoplayer.cpp

    desktop.files = desktop/desktop/tuxr.desktop
    desktop.path = /usr/share/applications

    icon16.files = desktop/desktop/16/tuxr.png
    icon16.path = /usr/share/icons/hicolor/16x16/apps

    icon22.files = desktop/desktop/22/tuxr.png
    icon22.path = /usr/share/icons/hicolor/22x22/apps

    icon48.files = desktop/desktop/48/tuxr.png
    icon48.path = /usr/share/icons/hicolor/48x48/apps

    icon64.files = desktop/desktop/64/tuxr.png
    icon64.path = /usr/share/icons/hicolor/64x64/apps
    
    target.path = /opt/tuxr/bin

    INSTALLS += \
        desktop \
        icon16 \
        icon22 \
        icon48 \
        icon64
}

INSTALLS += target
