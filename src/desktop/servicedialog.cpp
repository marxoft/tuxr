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

#include "servicedialog.h"
#include "instantiator.h"
#include "service.h"
#include "servicemodel.h"
#include <QAction>
#include <QDialogButtonBox>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

ServiceDialog::ServiceDialog(const QString &remoteId, QWidget *parent) :
    QDialog(parent),
    m_services(Instantiator::createServiceList(remoteId, this)),
    m_model(new ServiceModel(this)),
    m_view(new QTreeView(this)),
    m_toolBar(new QToolBar(this)),
    m_backAction(new QAction(QIcon::fromTheme("go-up"), tr("Go back"), this)),
    m_homeAction(new QAction(QIcon::fromTheme("go-home"), tr("Go home"), this)),
    m_reloadAction(new QAction(QIcon::fromTheme("view-refresh"), tr("Reload"), this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
                                     Qt::Horizontal, this)),
    m_layout(new QVBoxLayout(this))
{
    setWindowTitle(tr("Select service"));

    m_model->setServices(m_services);

    m_view->setModel(m_model),
    m_view->setSelectionBehavior(QTreeView::SelectRows);
    m_view->setEditTriggers(QTreeView::NoEditTriggers);
    m_view->setItemsExpandable(false);
    m_view->setIndentation(0);
    m_view->setUniformRowHeights(true);
    m_view->setAllColumnsShowFocus(true);

    m_toolBar->addAction(m_homeAction);
    m_toolBar->addAction(m_backAction);
    m_toolBar->addAction(m_reloadAction);

    m_backAction->setEnabled(false);
    m_backAction->setShortcuts(QKeySequence::Back);
    m_reloadAction->setShortcuts(QKeySequence::Refresh);

    m_layout->addWidget(m_view);
    m_layout->addWidget(m_toolBar);
    m_layout->addWidget(m_buttonBox);

    connect(m_view, SIGNAL(activated(QModelIndex)), this, SLOT(loadService(QModelIndex)));
    connect(m_backAction, SIGNAL(triggered()), this, SLOT(goBack()));
    connect(m_homeAction, SIGNAL(triggered()), this, SLOT(goHome()));
    connect(m_reloadAction, SIGNAL(triggered()), this, SLOT(reload()));
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QMap<int, QVariant> ServiceDialog::service() const {
    return m_model->itemData(m_view->currentIndex());
}

void ServiceDialog::goBack() {
    m_view->setRootIndex(m_view->rootIndex().parent());
    m_backAction->setEnabled(m_view->rootIndex().isValid());
    m_homeAction->setEnabled(m_backAction->isEnabled());
}

void ServiceDialog::goHome() {
    m_view->setRootIndex(QModelIndex());
    m_backAction->setEnabled(false);
    m_homeAction->setEnabled(false);
}

void ServiceDialog::reload() {
    m_model->reload(m_view->rootIndex());
}

void ServiceDialog::loadService(const QModelIndex &index) {
    if (index.data(ServiceModel::ServiceTypeRole) == Service::Bouquet) {
        m_model->reload(index);
        m_view->setRootIndex(index);
        m_backAction->setEnabled(true);
        m_homeAction->setEnabled(true);
    }
}
