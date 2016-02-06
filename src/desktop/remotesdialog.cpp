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

#include "remotesdialog.h"
#include "remoteconfigmodel.h"
#include "remoteconfigs.h"
#include "remotedialog.h"
#include "tuxr.h"
#include <QAction>
#include <QDialogButtonBox>
#include <QListView>
#include <QMenu>
#include <QVBoxLayout>

RemotesDialog::RemotesDialog(QWidget *parent) :
    QDialog(parent),
    m_model(new RemoteConfigModel(this)),
    m_view(new QListView(this)),
    m_contextMenu(new QMenu(this)),
    m_newAction(new QAction(QIcon::fromTheme("list-add"), tr("&New remote"), this)),
    m_editAction(new QAction(QIcon::fromTheme("document-edit"), tr("&Edit remote"), this)),
    m_removeAction(new QAction(QIcon::fromTheme("edit-clear"), tr("&Remove remote"), this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, this)),
    m_layout(new QVBoxLayout(this))
{
    setWindowTitle(tr("Remotes"));

    m_view->setModel(m_model);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);

    m_contextMenu->addAction(m_newAction);
    m_contextMenu->addAction(m_editAction);
    m_contextMenu->addAction(m_removeAction);

    m_layout->addWidget(m_view);
    m_layout->addWidget(m_buttonBox);

    connect(m_view, SIGNAL(activated(QModelIndex)), this, SLOT(editRemote(QModelIndex)));
    connect(m_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(m_newAction, SIGNAL(triggered()), this, SLOT(newRemote()));
    connect(m_editAction, SIGNAL(triggered()), this, SLOT(editRemote()));
    connect(m_removeAction, SIGNAL(triggered()), this, SLOT(removeRemote()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void RemotesDialog::editRemote() {
    if (m_view->currentIndex().isValid()) {
        editRemote(m_view->currentIndex());
    }
}

void RemotesDialog::editRemote(const QModelIndex &index) {
    RemoteDialog dialog(this);
    dialog.setRemoteType(TuxR::RemoteType(index.data(RemoteConfigModel::RemoteTypeRole).toInt()));
    dialog.setName(index.data(RemoteConfigModel::NameRole).toString());
    dialog.setAddress(index.data(RemoteConfigModel::AddressRole).toString());
    dialog.setStreamingPort(index.data(RemoteConfigModel::StreamingPortRole).toInt());
    dialog.setUsername(index.data(RemoteConfigModel::UsernameRole).toString());
    dialog.setPassword(index.data(RemoteConfigModel::PasswordRole).toString());

    if (dialog.exec() == QDialog::Accepted) {
        QMap<int, QVariant> config;
        config[RemoteConfigModel::RemoteTypeRole] = dialog.remoteType();
        config[RemoteConfigModel::NameRole] = dialog.name();
        config[RemoteConfigModel::AddressRole] = dialog.address();
        config[RemoteConfigModel::StreamingPortRole] = dialog.streamingPort();
        config[RemoteConfigModel::UsernameRole] = dialog.username();
        config[RemoteConfigModel::PasswordRole] = dialog.password();
        m_model->setItemData(index, config);
    }
}

void RemotesDialog::newRemote() {
    RemoteDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        RemoteConfigs::add(dialog.remoteType(), dialog.name(), dialog.address(),
                           dialog.streamingPort(), dialog.username(), dialog.password());
    }
}

void RemotesDialog::removeRemote() {
    if (m_view->currentIndex().isValid()) {
        RemoteConfigs::remove(m_view->currentIndex().data(RemoteConfigModel::IdRole).toString());
    }
}

void RemotesDialog::showContextMenu(const QPoint &pos) {
    if (m_view->currentIndex().isValid()) {
        m_contextMenu->popup(m_view->mapToGlobal(pos), m_newAction);
    }    
}
