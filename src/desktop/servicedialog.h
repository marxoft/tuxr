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

#ifndef SERVICEDIALOG_H
#define SERVICEDIALOG_H

#include <QDialog>

class ServiceList;
class ServiceModel;
class QDialogButtonBox;
class QToolBar;
class QTreeView;
class QVBoxLayout;

class ServiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceDialog(const QString &remoteId, QWidget *parent = 0);

    QMap<int, QVariant> service() const;

public Q_SLOTS:
    void goBack();
    void goHome();
    
    void reload();

private Q_SLOTS:
    void loadService(const QModelIndex &index);

private:
    ServiceList *m_services;
    ServiceModel *m_model;
    
    QTreeView *m_view;
    QToolBar *m_toolBar;

    QAction *m_backAction;
    QAction *m_homeAction;
    QAction *m_reloadAction;

    QDialogButtonBox *m_buttonBox;

    QVBoxLayout *m_layout;
};

#endif // SERVICEDIALOG_H
