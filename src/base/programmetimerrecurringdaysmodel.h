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

#ifndef PROGRAMMETIMERRECURRINGDAYSMODEL_H
#define PROGRAMMETIMERRECURRINGDAYSMODEL_H

#include "selectionmodel.h"
#include "programmetimer.h"

class ProgrammeTimerRecurringDaysModel : public SelectionModel
{
    Q_OBJECT

public:
    explicit ProgrammeTimerRecurringDaysModel(QObject *parent = 0) :
        SelectionModel(parent)
    {
        append(QDate::longDayName(1), ProgrammeTimer::Monday);
        append(QDate::longDayName(2), ProgrammeTimer::Tuesday);
        append(QDate::longDayName(3), ProgrammeTimer::Wednesday);
        append(QDate::longDayName(4), ProgrammeTimer::Thursday);
        append(QDate::longDayName(5), ProgrammeTimer::Friday);
        append(QDate::longDayName(6), ProgrammeTimer::Saturday);
        append(QDate::longDayName(7), ProgrammeTimer::Sunday);
    }
};

#endif // PROGRAMMETIMERRECURRINGDAYSMODEL_H
