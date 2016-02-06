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

#include "programmeview.h"
#include <QFormLayout>
#include <QLabel>

ProgrammeView::ProgrammeView(QWidget *parent) :
    QWidget(parent),
    m_titleLabel(new QLabel(this)),
    m_descriptionLabel(new QLabel(this)),
    m_startTimeLabel(new QLabel(this)),
    m_endTimeLabel(new QLabel(this)),
    m_channelTitleLabel(new QLabel(this)),
    m_layout(new QFormLayout(this))
{
    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);
    m_channelTitleLabel->setWordWrap(true);

    m_layout->addRow(tr("Title:"), m_titleLabel);
    m_layout->addRow(tr("Channel:"), m_channelTitleLabel);
    m_layout->addRow(tr("Start time:"), m_startTimeLabel);
    m_layout->addRow(tr("End time:"), m_endTimeLabel);
    m_layout->addRow(tr("Description:"), m_descriptionLabel);
    m_layout->setContentsMargins(0, 0, 0, 0);
}

QString ProgrammeView::title() const {
    return m_titleLabel->text();
}

void ProgrammeView::setTitle(const QString &t) {
    m_titleLabel->setText(t);
}

QString ProgrammeView::description() const {
    return m_descriptionLabel->text();
}

void ProgrammeView::setDescription(const QString &d) {
    m_descriptionLabel->setText(d);
}

QDateTime ProgrammeView::startTime() const {
    return QDateTime::fromString(m_startTimeLabel->text());
}

void ProgrammeView::setStartTime(const QDateTime &dt) {
    m_startTimeLabel->setText(dt.toString());
}

QDateTime ProgrammeView::endTime() const {
    return QDateTime::fromString(m_endTimeLabel->text());
}

void ProgrammeView::setEndTime(const QDateTime &dt) {
    m_endTimeLabel->setText(dt.toString());
}

int ProgrammeView::duration() const {
    const int secs = startTime().secsTo(endTime());

    if (secs > 60) {
        return secs / 60;
    }

    return 0;
}

QString ProgrammeView::channelTitle() const {
    return m_channelTitleLabel->text();
}

void ProgrammeView::setChannelTitle(const QString &t) {
    m_channelTitleLabel->setText(t);
}
