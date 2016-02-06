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

#include "remotedialog.h"
#include "remotetypemodel.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

RemoteDialog::RemoteDialog(QWidget *parent) :
    QDialog(parent),
    m_remoteTypeModel(new RemoteTypeModel(this)),
    m_remoteTypeSelector(new QComboBox(this)),
    m_nameEdit(new QLineEdit(this)),
    m_addressEdit(new QLineEdit(this)),
    m_usernameEdit(new QLineEdit(this)),
    m_passwordEdit(new QLineEdit(this)),
    m_portEdit(new QSpinBox(this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Save,
                                     Qt::Horizontal, this)),
    m_layout(new QFormLayout(this))
{
    setWindowTitle(tr("Remote configuration"));

    m_remoteTypeSelector->setModel(m_remoteTypeModel);

    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_portEdit->setMaximum(999999);

    m_layout->addRow(tr("Remote &type:"), m_remoteTypeSelector);
    m_layout->addRow(tr("Display &name:"), m_nameEdit);
    m_layout->addRow(tr("IP &address:"), m_addressEdit);
    m_layout->addRow(tr("&Streaming port:"), m_portEdit);
    m_layout->addRow(tr("&Username:"), m_usernameEdit);
    m_layout->addRow(tr("&Password:"), m_passwordEdit);
    m_layout->addRow(m_buttonBox);

    m_buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);

    connect(m_nameEdit, SIGNAL(textChanged(QString)), this, SLOT(onNameChanged(QString)));
    connect(m_addressEdit, SIGNAL(textChanged(QString)), this, SLOT(onAddressChanged(QString)));
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

TuxR::RemoteType RemoteDialog::remoteType() const {
    return TuxR::RemoteType(m_remoteTypeSelector->itemData(
                            m_remoteTypeSelector->currentIndex()).toInt());
}

void RemoteDialog::setRemoteType(TuxR::RemoteType t) {
    m_remoteTypeSelector->setCurrentIndex(m_remoteTypeSelector->findData(t));
}

QString RemoteDialog::name() const {
    return m_nameEdit->text();
}

void RemoteDialog::setName(const QString &n) {
    m_nameEdit->setText(n);
}

QString RemoteDialog::address() const {
    return m_addressEdit->text();
}

void RemoteDialog::setAddress(const QString &a) {
    m_addressEdit->setText(a);
}

int RemoteDialog::streamingPort() const {
    return m_portEdit->value();
}

void RemoteDialog::setStreamingPort(int p) {
    m_portEdit->setValue(p);
}

QString RemoteDialog::username() const {
    return m_usernameEdit->text();
}

void RemoteDialog::setUsername(const QString &u) {
    m_usernameEdit->setText(u);
}

QString RemoteDialog::password() const {
    return m_passwordEdit->text();
}

void RemoteDialog::setPassword(const QString &p) {
    m_passwordEdit->setText(p);
}

void RemoteDialog::onNameChanged(const QString &n) {
    m_buttonBox->button(QDialogButtonBox::Save)->setEnabled((!n.isEmpty()) && (!address().isEmpty()));
}

void RemoteDialog::onAddressChanged(const QString &a) {
    m_buttonBox->button(QDialogButtonBox::Save)->setEnabled((!a.isEmpty()) && (!name().isEmpty()));
}
