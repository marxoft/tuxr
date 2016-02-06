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

#ifndef REMOTEDIALOG_H
#define REMOTEDIALOG_H

#include "tuxr.h"
#include <QDialog>

class RemoteTypeModel;
class QComboBox;
class QDialogButtonBox;
class QFormLayout;
class QLineEdit;
class QSpinBox;

class RemoteDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(TuxR::RemoteType remoteType READ remoteType WRITE setRemoteType)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString address READ address WRITE setAddress)
    Q_PROPERTY(int streamingPort READ streamingPort WRITE setStreamingPort)
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    explicit RemoteDialog(QWidget *parent = 0);

    TuxR::RemoteType remoteType() const;
    void setRemoteType(TuxR::RemoteType t);

    QString name() const;
    void setName(const QString &n);

    QString address() const;
    void setAddress(const QString &a);

    int streamingPort() const;
    void setStreamingPort(int p);

    QString username() const;
    void setUsername(const QString &u);

    QString password() const;
    void setPassword(const QString &p);

private Q_SLOTS:
    void onNameChanged(const QString &n);
    void onAddressChanged(const QString &a);

private:
    RemoteTypeModel *m_remoteTypeModel;
    
    QComboBox *m_remoteTypeSelector;

    QLineEdit *m_nameEdit;
    QLineEdit *m_addressEdit;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;

    QSpinBox *m_portEdit;

    QDialogButtonBox *m_buttonBox;

    QFormLayout *m_layout;
};

#endif // REMOTEDIALOG_H
