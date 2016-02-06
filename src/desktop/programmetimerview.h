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

#ifndef PROGRAMMETIMERVIEW_H
#define PROGRAMMETIMERVIEW_H

#include <QWidget>

class ProgrammeTimerList;
class ProgrammeTimerModel;
class ProgrammeTimers;
class QMenu;
class QToolBar;
class QTreeView;
class QVBoxLayout;

class ProgrammeTimerView : public QWidget
{
    Q_OBJECT

public:
    explicit ProgrammeTimerView(const QString &remoteId, QWidget *parent = 0);
    
public Q_SLOTS:
    void reload();

private Q_SLOTS:
    void addTimer();
    void cleanupTimers();
    void clearTimers();
    void editTimer();
    void editTimer(const QModelIndex &index);
    void removeTimer();
    void removeTimer(const QModelIndex &index);

    void showContextMenu(const QPoint &pos);

    void onTimerListFinished();
    void onTimersFinished();

private:
    ProgrammeTimerList *m_timerList;
    ProgrammeTimers *m_timers;
    ProgrammeTimerModel *m_model;
    
    QTreeView *m_view;

    QToolBar *m_toolBar;

    QMenu *m_contextMenu;

    QAction *m_newAction;
    QAction *m_cleanupAction;
    QAction *m_clearAction;
    QAction *m_reloadAction;
    QAction *m_editAction;
    QAction *m_removeAction;
    
    QVBoxLayout *m_layout;
    
    QString m_remoteId;
};

#endif // PROGRAMMETIMERVIEW_H
