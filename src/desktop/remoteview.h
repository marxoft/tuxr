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

#ifndef REMOTEVIEW_H
#define REMOTEVIEW_H

#include "remote.h"
#include <QWidget>

class QGridLayout;

class RemoteView : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteView(const QString &remoteId, QWidget *parent = 0);
    
private:
    void addButton(const QString &text, const QString &icon, Remote::Key key, int row, int column);

private Q_SLOTS:
    void onButtonClicked();
    void onRemoteFinished();

private:
    Remote *m_remote;
  
    QGridLayout *m_layout;
};

#endif // REMOTEVIEW_H
