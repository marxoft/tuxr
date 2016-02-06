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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class RemoteConfigModel;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private Q_SLOTS:
    void onRemoteActionTriggered(QAction *action);

    void onRemoteChanged(const QModelIndex &index);
    void onRemoteAdded(const QString &id);
    void onRemoteRemoved(const QString &id);

    void setCurrentRemote(const QString &id);
    
    void showRemoteDialog();
    void showRemotesDialog();

private:
    RemoteConfigModel *m_remoteModel;
    
    QMenu *m_remoteMenu;
    QMenu *m_editMenu;
    
    QActionGroup *m_remoteGroup;

    QAction *m_newRemoteAction;
    QAction *m_quitAction;
    QAction *m_editRemotesAction;
};

#endif // MAINWINDOW_H
