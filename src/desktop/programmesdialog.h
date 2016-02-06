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

#ifndef PROGRAMMESDIALOG_H
#define PROGRAMMESDIALOG_H

#include <QDialog>

class ProgrammeList;
class ProgrammeModel;
class ProgrammeTimers;
class QDialogButtonBox;
class QMenu;
class QTreeView;
class QVBoxLayout;

class ProgrammesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgrammesDialog(const QString &remoteId, QWidget *parent = 0);

public Q_SLOTS:
    void reload(const QString &ref);

private Q_SLOTS:
    void showProgramme();
    void showProgramme(const QModelIndex &index);

    void setTimer();
    void setTimer(const QModelIndex &index);
    
    void showContextMenu(const QPoint &pos);

    void onProgrammesFinished();
    void onTimersFinished();

private:    
    ProgrammeList *m_programmes;
    ProgrammeTimers *m_timers;
    ProgrammeModel *m_model;
    
    QTreeView *m_view;

    QMenu *m_contextMenu;

    QAction *m_showAction;
    QAction *m_timerAction;

    QDialogButtonBox *m_buttonBox;
    
    QVBoxLayout *m_layout;
    
    QString m_remoteId;
};

#endif // PROGRAMMESDIALOG_H
