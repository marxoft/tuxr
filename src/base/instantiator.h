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

#ifndef INSTANTIATOR_H
#define INSTANTIATOR_H

#include <QObject>

class ProgrammeList;
class ProgrammeTimerList;
class ProgrammeTimers;
class Remote;
class ScreenShot;
class ServiceList;

class Instantiator : public QObject
{
    Q_OBJECT

public:
    explicit Instantiator(QObject *parent = 0);
    
public Q_SLOTS:
    static ProgrammeList* createProgrammeList(const QString &remoteId, QObject *parent = 0);
    static ProgrammeTimerList* createProgrammeTimerList(const QString &remoteId, QObject *parent = 0);
    static ProgrammeTimers* createProgrammeTimers(const QString &remoteId, QObject *parent = 0);
    static Remote* createRemote(const QString &remoteId, QObject *parent = 0);
    static ScreenShot* createScreenShot(const QString &remoteId, QObject *parent = 0);
    static ServiceList* createServiceList(const QString &remoteId, QObject *parent = 0);
};

#endif // INSTANTIATOR_H
