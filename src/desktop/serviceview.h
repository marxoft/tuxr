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

#ifndef SERVICEVIEW_H
#define SERVICEVIEW_H

#include <QWidget>

class Remote;
class ServiceList;
class ServiceModel;
class QMenu;
class QToolBar;
class QTreeView;
class QVBoxLayout;

class ServiceView : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceView(const QString &remoteId, QWidget *parent = 0);

public Q_SLOTS:
    void goBack();
    void goHome();
    
    void reload();

private Q_SLOTS:
    void loadService(const QModelIndex &index);
    
    void showProgrammes();
    void showProgrammes(const QModelIndex &index);

    void showContextMenu(const QPoint &pos);

    void onRemoteFinished();
    void onServicesFinished();

private:
    Remote* m_remote;
    ServiceList *m_services;
    ServiceModel *m_model;
    
    QTreeView *m_view;
    QToolBar *m_toolBar;
    QMenu *m_contextMenu;

    QAction *m_backAction;
    QAction *m_homeAction;
    QAction *m_reloadAction;
    QAction *m_programmesAction;

    QVBoxLayout *m_layout;
    
    QString m_remoteId;
};

#endif // SERVICEVIEW_H
