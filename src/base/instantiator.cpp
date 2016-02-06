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

#include "instantiator.h"
#include "enigma1programmelist.h"
#include "enigma1programmetimerlist.h"
#include "enigma1programmetimers.h"
#include "enigma1remote.h"
#include "enigma1screenshot.h"
#include "enigma1servicelist.h"
#include "remoteconfigs.h"
#include "tuxr.h"

Instantiator::Instantiator(QObject *parent) :
    QObject(parent)
{
}

ProgrammeList* Instantiator::createProgrammeList(const QString &remoteId, QObject *parent) {
    ProgrammeList *list;
    
    switch (RemoteConfigs::remoteType(remoteId)) {
    case TuxR::Enigma1:
        list = new Enigma1ProgrammeList(parent);
    default:
        list = new Enigma1ProgrammeList(parent);
    }
    
    list->setAddress(RemoteConfigs::address(remoteId));
    list->setUsername(RemoteConfigs::username(remoteId));
    list->setPassword(RemoteConfigs::password(remoteId));
    return list;
}

ProgrammeTimerList* Instantiator::createProgrammeTimerList(const QString &remoteId, QObject *parent) {
    ProgrammeTimerList *list;
    
    switch (RemoteConfigs::remoteType(remoteId)) {
    case TuxR::Enigma1:
        list = new Enigma1ProgrammeTimerList(parent);
    default:
        list = new Enigma1ProgrammeTimerList(parent);
    }
    
    list->setAddress(RemoteConfigs::address(remoteId));
    list->setUsername(RemoteConfigs::username(remoteId));
    list->setPassword(RemoteConfigs::password(remoteId));
    return list;
}

ProgrammeTimers* Instantiator::createProgrammeTimers(const QString &remoteId, QObject *parent) {
    ProgrammeTimers *timers;
    
    switch (RemoteConfigs::remoteType(remoteId)) {
    case TuxR::Enigma1:
        timers = new Enigma1ProgrammeTimers(parent);
    default:
        timers = new Enigma1ProgrammeTimers(parent);
    }
    
    timers->setAddress(RemoteConfigs::address(remoteId));
    timers->setUsername(RemoteConfigs::username(remoteId));
    timers->setPassword(RemoteConfigs::password(remoteId));
    return timers;
}

Remote* Instantiator::createRemote(const QString &remoteId, QObject *parent) {
    Remote *remote;
    
    switch (RemoteConfigs::remoteType(remoteId)) {
    case TuxR::Enigma1:
        remote = new Enigma1Remote(parent);
    default:
        remote = new Enigma1Remote(parent);
    }
    
    remote->setAddress(RemoteConfigs::address(remoteId));
    remote->setUsername(RemoteConfigs::username(remoteId));
    remote->setPassword(RemoteConfigs::password(remoteId));
    return remote;
}

ScreenShot* Instantiator::createScreenShot(const QString &remoteId, QObject *parent) {
    ScreenShot *screenShot;
    
    switch (RemoteConfigs::remoteType(remoteId)) {
    case TuxR::Enigma1:
        screenShot = new Enigma1ScreenShot(parent);
    default:
        screenShot = new Enigma1ScreenShot(parent);
    }
    
    screenShot->setAddress(RemoteConfigs::address(remoteId));
    screenShot->setUsername(RemoteConfigs::username(remoteId));
    screenShot->setPassword(RemoteConfigs::password(remoteId));
    return screenShot;
}

ServiceList* Instantiator::createServiceList(const QString &remoteId, QObject *parent) {
    ServiceList *list;
    
    switch (RemoteConfigs::remoteType(remoteId)) {
    case TuxR::Enigma1:
        list = new Enigma1ServiceList(parent);
    default:
        list = new Enigma1ServiceList(parent);
    }
    
    list->setAddress(RemoteConfigs::address(remoteId));
    list->setUsername(RemoteConfigs::username(remoteId));
    list->setPassword(RemoteConfigs::password(remoteId));
    return list;
}
