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

#include "programmedialog.h"
#include "programmeview.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>

ProgrammeDialog::ProgrammeDialog(QWidget *parent) :
    QDialog(parent),
    m_view(new ProgrammeView(this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, this)),
    m_layout(new QVBoxLayout(this))
{
    setWindowTitle(tr("Programme details"));

    m_layout->addWidget(m_view);
    m_layout->addWidget(m_buttonBox);
    
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QString ProgrammeDialog::title() const {
    return m_view->title();
}

void ProgrammeDialog::setTitle(const QString &t) {
    m_view->setTitle(t);
}

QString ProgrammeDialog::description() const {
    return m_view->description();
}

void ProgrammeDialog::setDescription(const QString &d) {
    m_view->setDescription(d);
}

QDateTime ProgrammeDialog::startTime() const {
    return m_view->startTime();
}

void ProgrammeDialog::setStartTime(const QDateTime &dt) {
    m_view->setStartTime(dt);
}

QDateTime ProgrammeDialog::endTime() const {
    return m_view->endTime();
}

void ProgrammeDialog::setEndTime(const QDateTime &dt) {
    m_view->setEndTime(dt);
}

int ProgrammeDialog::duration() const {
    return m_view->duration();
}

QString ProgrammeDialog::channelTitle() const {
    return m_view->channelTitle();
}

void ProgrammeDialog::setChannelTitle(const QString &t) {
    m_view->setChannelTitle(t);
}
