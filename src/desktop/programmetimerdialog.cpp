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

#include "programmetimerdialog.h"
#include "programmetimer.h"
#include "programmetimeractionmodel.h"
#include "programmetimerafteractionmodel.h"
#include "programmetimereventtypemodel.h"
#include "service.h"
#include "servicedialog.h"
#include "servicemodel.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

ProgrammeTimerDialog::ProgrammeTimerDialog(const QString &remoteId, QWidget *parent) :
    QDialog(parent),
    m_timer(new ProgrammeTimer(this)),
    m_eventTypeModel(new ProgrammeTimerEventTypeModel(this)),
    m_actionModel(new ProgrammeTimerActionModel(this)),
    m_afterActionModel(new ProgrammeTimerAfterActionModel(this)),
    m_eventTypeSelector(new QComboBox(this)),
    m_actionSelector(new QComboBox(this)),
    m_afterActionSelector(new QComboBox(this)),
    m_startTimeEdit(new QDateTimeEdit(this)),
    m_endTimeEdit(new QDateTimeEdit(this)),
    m_daysWidget(new QWidget(this)),
    m_daysLayout(new QGridLayout(m_daysWidget)),
    m_mondayCheckBox(new QCheckBox(QDate::longDayName(1), this)),
    m_tuesdayCheckBox(new QCheckBox(QDate::longDayName(2), this)),
    m_wednesdayCheckBox(new QCheckBox(QDate::longDayName(3), this)),
    m_thursdayCheckBox(new QCheckBox(QDate::longDayName(4), this)),
    m_fridayCheckBox(new QCheckBox(QDate::longDayName(5), this)),
    m_saturdayCheckBox(new QCheckBox(QDate::longDayName(6), this)),
    m_sundayCheckBox(new QCheckBox(QDate::longDayName(7), this)),
    m_serviceButton(new QPushButton(tr("None chosen"), this)),
    m_descriptionEdit(new QLineEdit(this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Save,
                                     Qt::Horizontal, this)),
    m_layout(new QFormLayout(this)),
    m_remoteId(remoteId)
{
    setWindowTitle(tr("Timer details"));

    m_eventTypeSelector->setModel(m_eventTypeModel);
    m_actionSelector->setModel(m_actionModel);
    m_afterActionSelector->setModel(m_afterActionModel);

    const QDateTime current = QDateTime::currentDateTime();
    m_startTimeEdit->setDateTime(current);
    m_endTimeEdit->setDateTime(current);

    m_daysWidget->setEnabled(false);

    m_daysLayout->addWidget(m_mondayCheckBox, 0, 0);
    m_daysLayout->addWidget(m_tuesdayCheckBox, 0, 1);
    m_daysLayout->addWidget(m_wednesdayCheckBox, 0, 2);
    m_daysLayout->addWidget(m_thursdayCheckBox, 0, 3);
    m_daysLayout->addWidget(m_fridayCheckBox, 1, 0);
    m_daysLayout->addWidget(m_saturdayCheckBox, 1, 1);
    m_daysLayout->addWidget(m_sundayCheckBox, 1, 2);
    m_daysLayout->setContentsMargins(0, 0, 0, 0);

    m_layout->addRow(tr("Event &type:"), m_eventTypeSelector);
    m_layout->addRow(tr("&Action:"), m_actionSelector);
    m_layout->addRow(tr("A&fter:"), m_afterActionSelector);
    m_layout->addRow(tr("&Start time:"), m_startTimeEdit);
    m_layout->addRow(tr("&End time:"), m_endTimeEdit);
    m_layout->addRow(tr("&Recurring days:"), m_daysWidget);
    m_layout->addRow(tr("&Channel:"), m_serviceButton);
    m_layout->addRow(tr("&Description:"), m_descriptionEdit);
    m_layout->addWidget(m_buttonBox);

    connect(m_eventTypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onEventTypeChanged(int)));
    connect(m_serviceButton, SIGNAL(clicked()), this, SLOT(showServices()));
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QString ProgrammeTimerDialog::description() const {
    return m_descriptionEdit->text();
}

void ProgrammeTimerDialog::setDescription(const QString &d) {
    m_descriptionEdit->setText(d);
}

QDateTime ProgrammeTimerDialog::startTime() const {
    return m_startTimeEdit->dateTime();
}

void ProgrammeTimerDialog::setStartTime(const QDateTime &dt) {
    m_startTimeEdit->setDateTime(dt);
}

QDateTime ProgrammeTimerDialog::endTime() const {
    return m_endTimeEdit->dateTime();
}

void ProgrammeTimerDialog::setEndTime(const QDateTime &dt) {
    m_endTimeEdit->setDateTime(dt);
}

int ProgrammeTimerDialog::duration() const {
    const int secs =
    eventType() == ProgrammeTimer::RecurringEvent ? startTime().time().secsTo(endTime().time())
                                                  : startTime().secsTo(endTime());

    if (secs > 60) {
        return secs / 60;
    }

    return 0;
}

QString ProgrammeTimerDialog::channelId() const {
    return m_timer->channelId();
}

void ProgrammeTimerDialog::setChannelId(const QString &i) {
    m_timer->setChannelId(i);
}

QString ProgrammeTimerDialog::channelTitle() const {
    return m_timer->channelTitle();
}

void ProgrammeTimerDialog::setChannelTitle(const QString &t) {
    m_timer->setChannelTitle(t);
    m_serviceButton->setText(t);
}

ProgrammeTimer::EventType ProgrammeTimerDialog::eventType() const {
    return ProgrammeTimer::EventType(m_eventTypeSelector->itemData(
                                     m_eventTypeSelector->currentIndex()).toInt());
}

void ProgrammeTimerDialog::setEventType(ProgrammeTimer::EventType t) {
    m_eventTypeSelector->setCurrentIndex(m_eventTypeSelector->findData(t));
}

ProgrammeTimer::Days ProgrammeTimerDialog::recurringDays() const {
    ProgrammeTimer::Days days;

    if (m_mondayCheckBox->isChecked()) {
        days |= ProgrammeTimer::Monday;
    }
    
    if (m_tuesdayCheckBox->isChecked()) {
        days |= ProgrammeTimer::Tuesday;
    }
    
    if (m_wednesdayCheckBox->isChecked()) {
        days |= ProgrammeTimer::Wednesday;
    }
    
    if (m_thursdayCheckBox->isChecked()) {
        days |= ProgrammeTimer::Thursday;
    }
    
    if (m_fridayCheckBox->isChecked()) {
        days |= ProgrammeTimer::Friday;
    }
    
    if (m_saturdayCheckBox->isChecked()) {
        days |= ProgrammeTimer::Saturday;
    }
    
    if (m_sundayCheckBox->isChecked()) {
        days |= ProgrammeTimer::Sunday;
    }

    return days;
}

void ProgrammeTimerDialog::setRecurringDays(ProgrammeTimer::Days d) {
    m_mondayCheckBox->setChecked(d & ProgrammeTimer::Monday);
    m_tuesdayCheckBox->setChecked(d & ProgrammeTimer::Tuesday);
    m_wednesdayCheckBox->setChecked(d & ProgrammeTimer::Wednesday);
    m_thursdayCheckBox->setChecked(d & ProgrammeTimer::Thursday);
    m_fridayCheckBox->setChecked(d & ProgrammeTimer::Friday);
    m_saturdayCheckBox->setChecked(d & ProgrammeTimer::Saturday);
    m_sundayCheckBox->setChecked(d & ProgrammeTimer::Sunday);
}

ProgrammeTimer::Action ProgrammeTimerDialog::action() const {
    return ProgrammeTimer::Action(m_actionSelector->itemData(
                                  m_actionSelector->currentIndex()).toInt());
}

void ProgrammeTimerDialog::setAction(ProgrammeTimer::Action a) {
    m_actionSelector->setCurrentIndex(m_actionSelector->findData(a));
}

ProgrammeTimer::Action ProgrammeTimerDialog::afterAction() const {
    return ProgrammeTimer::Action(m_afterActionSelector->itemData(
                                  m_afterActionSelector->currentIndex()).toInt());
}

void ProgrammeTimerDialog::setAfterAction(ProgrammeTimer::Action a) {
    m_afterActionSelector->setCurrentIndex(m_afterActionSelector->findData(a));
}

void ProgrammeTimerDialog::showServices() {
    ServiceDialog dialog(m_remoteId, this);
    dialog.reload();

    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.service().value(ServiceModel::ServiceTypeRole) == Service::Channel) {
            setChannelId(dialog.service().value(ServiceModel::IdRole).toString());
            setChannelTitle(dialog.service().value(ServiceModel::TitleRole).toString());
        }
        else {
            QMessageBox::critical(this, tr("Error"), tr("The chosen service is not a channel."));
        }
    }
}

void ProgrammeTimerDialog::onEventTypeChanged(int index) {
    m_daysWidget->setEnabled(index == 1);
}
