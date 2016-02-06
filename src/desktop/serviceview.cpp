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

#include "serviceview.h"
#include "instantiator.h"
#include "programmesdialog.h"
#include "remote.h"
#include "service.h"
#include "servicemodel.h"
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

ServiceView::ServiceView(const QString &remoteId, QWidget *parent) :
    QWidget(parent),
    m_remote(Instantiator::createRemote(remoteId, this)),
    m_services(Instantiator::createServiceList(remoteId, this)),
    m_model(new ServiceModel(this)),
    m_view(new QTreeView(this)),
    m_toolBar(new QToolBar(this)),
    m_contextMenu(new QMenu(this)),
    m_backAction(new QAction(QIcon::fromTheme("go-up"), tr("Go up"), this)),
    m_homeAction(new QAction(QIcon::fromTheme("go-home"), tr("Go home"), this)),
    m_reloadAction(new QAction(QIcon::fromTheme("view-refresh"), tr("Reload"), this)),
    m_programmesAction(new QAction(QIcon::fromTheme("view-media-playlist"),
                       tr("Show EPG"), this)),
    m_layout(new QVBoxLayout(this)),
    m_remoteId(remoteId)
{
    m_model->setServices(m_services);

    m_view->setModel(m_model),
    m_view->setSelectionBehavior(QTreeView::SelectRows);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    m_view->setEditTriggers(QTreeView::NoEditTriggers);
    m_view->setItemsExpandable(false);
    m_view->setIndentation(0);
    m_view->setUniformRowHeights(true);
    m_view->setAllColumnsShowFocus(true);

    m_toolBar->addAction(m_homeAction);
    m_toolBar->addAction(m_backAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_reloadAction);

    m_contextMenu->addAction(m_programmesAction);

    m_backAction->setEnabled(false);
    m_backAction->setShortcuts(QKeySequence::Back);
    m_reloadAction->setShortcuts(QKeySequence::Refresh);

    m_layout->addWidget(m_view);
    m_layout->addWidget(m_toolBar);

    connect(m_remote, SIGNAL(finished()), this, SLOT(onRemoteFinished()));
    connect(m_services, SIGNAL(finished()), this, SLOT(onServicesFinished()));
    connect(m_view, SIGNAL(activated(QModelIndex)), this, SLOT(loadService(QModelIndex)));
    connect(m_view, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    connect(m_backAction, SIGNAL(triggered()), this, SLOT(goBack()));
    connect(m_homeAction, SIGNAL(triggered()), this, SLOT(goHome()));
    connect(m_reloadAction, SIGNAL(triggered()), this, SLOT(reload()));
    connect(m_programmesAction, SIGNAL(triggered()), this, SLOT(showProgrammes()));
}

void ServiceView::goBack() {
    m_view->setRootIndex(m_view->rootIndex().parent());
    m_backAction->setEnabled(m_view->rootIndex().isValid());
    m_homeAction->setEnabled(m_backAction->isEnabled());
}

void ServiceView::goHome() {
    m_view->setRootIndex(QModelIndex());
    m_backAction->setEnabled(false);
    m_homeAction->setEnabled(false);
}

void ServiceView::reload() {
    m_model->reload(m_view->rootIndex());
}

void ServiceView::loadService(const QModelIndex &index) {
    if (index.data(ServiceModel::ServiceTypeRole) == Service::Bouquet) {
        m_model->reload(index);
        m_view->setRootIndex(index);
        m_backAction->setEnabled(true);
        m_homeAction->setEnabled(true);
    }
    else {
        m_remote->changeChannel(index.data(ServiceModel::IdRole).toString());
    }
}

void ServiceView::showProgrammes() {
    if (m_view->currentIndex().isValid()) {
        showProgrammes(m_view->currentIndex());
    }
}

void ServiceView::showProgrammes(const QModelIndex &index) {
    ProgrammesDialog dialog(m_remoteId, this);
    dialog.setWindowTitle(tr("EPG - %1").arg(index.data(ServiceModel::TitleRole).toString()));
    dialog.reload(index.data(ServiceModel::IdRole).toString());
    dialog.exec();
}

void ServiceView::showContextMenu(const QPoint &pos) {
    if (m_view->currentIndex().isValid()) {
        m_contextMenu->popup(m_view->mapToGlobal(pos), m_programmesAction);
    }
}

void ServiceView::onRemoteFinished() {
    if (m_remote->status() == Request::Error) {
        QMessageBox::critical(this, tr("Error"), m_remote->errorString());
    }
}

void ServiceView::onServicesFinished() {
    if (m_services->status() == Request::Error) {
        QMessageBox::critical(this, tr("Error"), m_services->errorString());
    }
}
