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

#include "activecolormodel.h"
#include "datetimeutils.h"
#include "enigma1programmelist.h"
#include "enigma1programmetimerlist.h"
#include "enigma1programmetimers.h"
#include "enigma1remote.h"
#include "enigma1screenshot.h"
#include "enigma1servicelist.h"
#include "instantiator.h"
#include "pixmapitem.h"
#include "programme.h"
#include "programmemodel.h"
#include "programmetimer.h"
#include "programmetimeractionmodel.h"
#include "programmetimerafteractionmodel.h"
#include "programmetimereventtypemodel.h"
#include "programmetimermodel.h"
#include "programmetimerrecurringdaysmodel.h"
#include "remoteconfigmodel.h"
#include "remoteconfigs.h"
#include "remotekeymodel.h"
#include "remotemacromodel.h"
#include "remotetypemodel.h"
#include "screenorientationmodel.h"
#include "screenshotimagetypemodel.h"
#include "service.h"
#include "servicemodel.h"
#include "settings.h"
#include "tuxr.h"
#include <QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QGLWidget>
#include <qdeclarative.h>

#define VERSION_NUMBER "0.0.1"
#define DATE_TIME_FORMAT "dd/MM/yyyy HH:mm"
#define DATE_FORMAT "dd/MM/yyyy"
#define TIME_FORMAT "HH:mm"

void registerTypes() {
    qmlRegisterType<ActiveColorModel>("TuxR", 1, 0, "ActiveColorModel");
    qmlRegisterType<Enigma1ProgrammeList>("TuxR", 1, 0, "Enigma1ProgrammeList");
    qmlRegisterType<Enigma1ProgrammeTimerList>("TuxR", 1, 0, "Enigma1ProgrammeTimerList");
    qmlRegisterType<Enigma1ProgrammeTimers>("TuxR", 1, 0, "Enigma1ProgrammeTimers");
    qmlRegisterType<Enigma1Remote>("TuxR", 1, 0, "Enigma1Remote");
    qmlRegisterType<Enigma1ScreenShot>("TuxR", 1, 0, "Enigma1ScreenShot");
    qmlRegisterType<Enigma1ServiceList>("TuxR", 1, 0, "Enigma1ServiceList");
    qmlRegisterType<PixmapItem>("TuxR", 1, 0, "Pixmap");
    qmlRegisterType<Programme>("TuxR", 1, 0, "Programme");
    qmlRegisterType<ProgrammeModel>("TuxR", 1, 0, "ProgrammeModel");
    qmlRegisterType<ProgrammeTimer>("TuxR", 1, 0, "ProgrammeTimer");
    qmlRegisterType<ProgrammeTimerActionModel>("TuxR", 1, 0, "ProgrammeTimerActionModel");
    qmlRegisterType<ProgrammeTimerAfterActionModel>("TuxR", 1, 0, "ProgrammeTimerAfterActionModel");
    qmlRegisterType<ProgrammeTimerEventTypeModel>("TuxR", 1, 0, "ProgrammeTimerEventTypeModel");
    qmlRegisterType<ProgrammeTimerModel>("TuxR", 1, 0, "ProgrammeTimerModel");
    qmlRegisterType<ProgrammeTimerRecurringDaysModel>("TuxR", 1, 0, "ProgrammeTimerRecurringDaysModel");
    qmlRegisterType<RemoteConfigModel>("TuxR", 1, 0, "RemoteConfigModel");
    qmlRegisterType<RemoteKeyModel>("TuxR", 1, 0, "RemoteKeyModel");
    qmlRegisterType<RemoteMacroModel>("TuxR", 1, 0, "RemoteMacroModel");
    qmlRegisterType<RemoteTypeModel>("TuxR", 1, 0, "RemoteTypeModel");
    qmlRegisterType<Request>("TuxR", 1, 0, "Request");
    qmlRegisterType<ScreenOrientationModel>("TuxR", 1, 0, "ScreenOrientationModel");
    qmlRegisterType<ScreenShotImageTypeModel>("TuxR", 1, 0, "ScreenShotImageTypeModel");
    qmlRegisterType<SelectionModel>("TuxR", 1, 0, "SelectionModel");
    qmlRegisterType<Service>("TuxR", 1, 0, "Service");
    qmlRegisterType<ServiceModel>("TuxR", 1, 0, "ServiceModel");

    qmlRegisterUncreatableType<ProgrammeList>("TuxR", 1, 0, "ProgrammeList", "");
    qmlRegisterUncreatableType<ProgrammeTimerList>("TuxR", 1, 0, "ProgrammeTimerList", "");
    qmlRegisterUncreatableType<ProgrammeTimers>("TuxR", 1, 0, "ProgrammeTimers", "");
    qmlRegisterUncreatableType<Remote>("TuxR", 1, 0, "Remote", "");
    qmlRegisterUncreatableType<ScreenShot>("TuxR", 1, 0, "ScreenShot", "");
    qmlRegisterUncreatableType<ServiceList>("TuxR", 1, 0, "ProgrammeList", "");
    qmlRegisterUncreatableType<TuxR>("TuxR", 1, 0, "TuxR", "");
    
    qRegisterMetaType<Request::Status>("Request::Status");
}

Q_DECL_EXPORT int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("TuxR");
    app.setApplicationName("TuxR");
    app.setApplicationVersion(VERSION_NUMBER);

    registerTypes();
    
    DateTimeUtils utils;
    Instantiator instantiator;
    RemoteKeys keys;

    QScopedPointer<RemoteConfigs> remotes(RemoteConfigs::instance());
    QScopedPointer<Settings> settings(Settings::instance());

    QDeclarativeView view;
    QDeclarativeContext *context = view.rootContext();
    context->setContextProperty("utils", &utils);
    context->setContextProperty("instantiator", &instantiator);
    context->setContextProperty("remoteKeys", &keys);
    context->setContextProperty("remotes", remotes.data());
    context->setContextProperty("settings", settings.data());
    context->setContextProperty("DATE_TIME_FORMAT", DATE_TIME_FORMAT);
    context->setContextProperty("DATE_FORMAT", DATE_FORMAT);
    context->setContextProperty("TIME_FORMAT", TIME_FORMAT);
    context->setContextProperty("VERSION_NUMBER", VERSION_NUMBER);

    view.setViewport(new QGLWidget);
    view.setSource(QUrl::fromLocalFile("/opt/tuxr/qml/main.qml"));
    view.showFullScreen();

    return app.exec();
}
