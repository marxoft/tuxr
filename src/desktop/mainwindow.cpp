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

#include "mainwindow.h"
#include "remoteconfigmodel.h"
#include "remoteconfigs.h"
#include "remotedialog.h"
#include "remotesdialog.h"
#include "remotewidget.h"
#include "settings.h"
#include <QActionGroup>
#include <QCoreApplication>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_remoteModel(new RemoteConfigModel(this)),
    m_remoteMenu(new QMenu(tr("&Remote"), this)),
    m_editMenu(new QMenu(tr("&Edit"), this)),
    m_remoteGroup(new QActionGroup(this)),
    m_newRemoteAction(new QAction(QIcon::fromTheme("list-add"), tr("&New remote"), this)),
    m_quitAction(new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this)),
    m_editRemotesAction(new QAction(QIcon::fromTheme("document-properties"), tr("&Remotes"), this))
{
    setWindowTitle("TuxR");
    setMinimumSize(QSize(1000, 600));

    menuBar()->addMenu(m_remoteMenu);
    menuBar()->addMenu(m_editMenu);

    m_remoteModel->reload();

    for (int i = 0; i < m_remoteModel->rowCount(); i++) {
        const QModelIndex &index = m_remoteModel->index(i);
        QAction *action = new QAction(index.data(RemoteConfigModel::NameRole).toString(), m_remoteGroup);
        action->setData(index.data(RemoteConfigModel::IdRole));
        action->setCheckable(true);
        m_remoteMenu->addAction(action);
    }

    m_remoteGroup->setExclusive(true);

    m_newRemoteAction->setShortcuts(QKeySequence::New);
    m_quitAction->setShortcuts(QKeySequence::Quit);
    m_editRemotesAction->setShortcuts(QKeySequence::Preferences);

    m_remoteMenu->addAction(m_newRemoteAction);
    m_remoteMenu->addAction(m_quitAction);

    m_editMenu->addAction(m_editRemotesAction);

    connect(m_remoteGroup, SIGNAL(triggered(QAction*)), this, SLOT(onRemoteActionTriggered(QAction*)));
    connect(m_newRemoteAction, SIGNAL(triggered()), this, SLOT(showRemoteDialog()));
    connect(m_quitAction, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(quit()));
    connect(m_editRemotesAction, SIGNAL(triggered()), this, SLOT(showRemotesDialog()));
    connect(m_remoteModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(onRemoteChanged(QModelIndex)));
    connect(RemoteConfigs::instance(), SIGNAL(added(QString)), this, SLOT(onRemoteAdded(QString)));
    connect(RemoteConfigs::instance(), SIGNAL(removed(QString)), this, SLOT(onRemoteRemoved(QString)));
    connect(Settings::instance(), SIGNAL(currentRemoteChanged(QString)),
            this, SLOT(setCurrentRemote(QString)));

    const QString id = Settings::instance()->currentRemote();

    foreach (QAction* action, m_remoteGroup->actions()) {
        if (action->data() == id) {
            action->setChecked(true);
            setCurrentRemote(id);
            break;
        }
    }
}

void MainWindow::onRemoteActionTriggered(QAction *action) {
    Settings::instance()->setCurrentRemote(action->data().toString());
}

void MainWindow::onRemoteChanged(const QModelIndex &index) {
    foreach (QAction *action, m_remoteGroup->actions()) {
        if (action->data() == index.data(RemoteConfigModel::IdRole)) {
            action->setText(index.data(RemoteConfigModel::NameRole).toString());
            break;
        }
    }

    if (index.data(RemoteConfigModel::IdRole) == Settings::instance()->currentRemote()) {
        setWindowTitle("TuxR - " + index.data(RemoteConfigModel::NameRole).toString());
    }
}

void MainWindow::onRemoteAdded(const QString &id) {
    QAction *action = new QAction(RemoteConfigs::name(id), m_remoteGroup);
    action->setData(id);
    action->setCheckable(true);
    m_remoteMenu->insertAction(m_newRemoteAction, action);
}

void MainWindow::onRemoteRemoved(const QString &id) {
    foreach (QAction *action, m_remoteGroup->actions()) {
        if (action->data() == id) {
            m_remoteGroup->removeAction(action);
            action->deleteLater();

            if (Settings::instance()->currentRemote() == id) {
                if (!m_remoteGroup->actions().isEmpty()) {
                    m_remoteGroup->actions().first()->trigger();
                }
                else {
                    setCentralWidget(0);
                }
            }

            break;
        }
    }
}

void MainWindow::setCurrentRemote(const QString &id) {
    setCentralWidget(new RemoteWidget(id, this));
    setWindowTitle("TuxR - " + RemoteConfigs::name(id));
}

void MainWindow::showRemoteDialog() {
    RemoteDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        RemoteConfigs::add(dialog.remoteType(), dialog.name(), dialog.address(), dialog.streamingPort(),
                           dialog.username(), dialog.password());
    }
}

void MainWindow::showRemotesDialog() {
    RemotesDialog(this).exec();
}
