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

#ifndef PROGRAMMETIMERDIALOG_H
#define PROGRAMMETIMERDIALOG_H

#include "programmetimer.h"
#include <QDialog>
#include <QMap>
#include <QVariant>

class ProgrammeTimerActionModel;
class ProgrammeTimerAfterActionModel;
class ProgrammeTimerEventTypeModel;
class QCheckBox;
class QComboBox;
class QDateTimeEdit;
class QDialogButtonBox;
class QFormLayout;
class QGridLayout;
class QLineEdit;
class QPushButton;

class ProgrammeTimerDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime)
    Q_PROPERTY(QDateTime endTime READ endTime WRITE setEndTime)
    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(QString channelId READ channelId WRITE setChannelId)
    Q_PROPERTY(QString channelTitle READ channelTitle WRITE setChannelTitle)
    Q_PROPERTY(ProgrammeTimer::EventType eventType READ eventType WRITE setEventType)
    Q_PROPERTY(ProgrammeTimer::Days recurringDays READ recurringDays WRITE setRecurringDays)
    Q_PROPERTY(ProgrammeTimer::Action action READ action WRITE setAction)
    Q_PROPERTY(ProgrammeTimer::Action afterAction READ afterAction WRITE setAfterAction)

public:
    explicit ProgrammeTimerDialog(const QString &remoteId, QWidget *parent = 0);
    
    QString description() const;
    void setDescription(const QString &d);

    QDateTime startTime() const;
    void setStartTime(const QDateTime &dt);

    QDateTime endTime() const;
    void setEndTime(const QDateTime &dt);

    int duration() const;

    QString channelId() const;
    void setChannelId(const QString &i);

    QString channelTitle() const;
    void setChannelTitle(const QString &t);

    ProgrammeTimer::EventType eventType() const;
    void setEventType(ProgrammeTimer::EventType t);

    ProgrammeTimer::Days recurringDays() const;
    void setRecurringDays(ProgrammeTimer::Days d);

    ProgrammeTimer::Action action() const;
    void setAction(ProgrammeTimer::Action a);

    ProgrammeTimer::Action afterAction() const;
    void setAfterAction(ProgrammeTimer::Action a);

private Q_SLOTS:
    void showServices();

    void onEventTypeChanged(int index);

private:    
    ProgrammeTimer *m_timer;
    ProgrammeTimerEventTypeModel *m_eventTypeModel;
    ProgrammeTimerActionModel *m_actionModel;
    ProgrammeTimerAfterActionModel *m_afterActionModel;

    QComboBox *m_eventTypeSelector;
    QComboBox *m_actionSelector;
    QComboBox *m_afterActionSelector;

    QDateTimeEdit *m_startTimeEdit;
    QDateTimeEdit *m_endTimeEdit;

    QWidget *m_daysWidget;
    QGridLayout *m_daysLayout;
    
    QCheckBox *m_mondayCheckBox;
    QCheckBox *m_tuesdayCheckBox;
    QCheckBox *m_wednesdayCheckBox;
    QCheckBox *m_thursdayCheckBox;
    QCheckBox *m_fridayCheckBox;
    QCheckBox *m_saturdayCheckBox;
    QCheckBox *m_sundayCheckBox;

    QPushButton *m_serviceButton;

    QLineEdit *m_descriptionEdit;

    QDialogButtonBox *m_buttonBox;
    
    QFormLayout *m_layout;
    
    QString m_remoteId;
};
    
#endif // PROGRAMMETIMERDIALOG_H
