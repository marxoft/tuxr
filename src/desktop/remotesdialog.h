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

#ifndef REMOTESDIALOG_H
#define REMOTESDIALOG_H

#include <QDialog>

class RemoteConfigModel;
class QDialogButtonBox;
class QListView;
class QMenu;
class QVBoxLayout;

class RemotesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemotesDialog(QWidget *parent = 0);

private Q_SLOTS:
    void editRemote();
    void editRemote(const QModelIndex &index);
    void newRemote();
    void removeRemote();

    void showContextMenu(const QPoint &pos);

private:
    RemoteConfigModel *m_model;
    
    QListView *m_view;

    QMenu *m_contextMenu;

    QAction *m_newAction;
    QAction *m_editAction;
    QAction *m_removeAction;

    QDialogButtonBox *m_buttonBox;

    QVBoxLayout *m_layout;
};

#endif // REMOTESDIALOG_H
