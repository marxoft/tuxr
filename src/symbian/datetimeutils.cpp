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

#include "datetimeutils.h"

DateTimeUtils::DateTimeUtils(QObject *parent) :
    QObject(parent)
{
}

QDateTime DateTimeUtils::currentDateTime() {
    return QDateTime::currentDateTime();
}

QDateTime DateTimeUtils::dateTime(int year, int month, int day, int hour, int minute, int second) {
    return QDateTime(QDate(year, month, day), QTime(hour, minute, second));
}

int DateTimeUtils::day(const QDateTime &dt) {
    return dt.date().day();
}

int DateTimeUtils::month(const QDateTime &dt) {
    return dt.date().month();
}

int DateTimeUtils::year(const QDateTime &dt) {
    return dt.date().year();
}

int DateTimeUtils::hour(const QDateTime &dt) {
    return dt.time().hour();
}

int DateTimeUtils::minute(const QDateTime &dt) {
    return dt.time().minute();
}

int DateTimeUtils::second(const QDateTime &dt) {
    return dt.time().second();
}
