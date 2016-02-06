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

#ifndef REMOTEMACROMODEL_H
#define REMOTEMACROMODEL_H

#include "selectionmodel.h"

class RemoteMacroModel : public SelectionModel
{
    Q_OBJECT
    
public:
    explicit RemoteMacroModel(QObject *parent = 0);
        
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    
    Q_INVOKABLE void append(const QString &text, const QVariant &keys);
    Q_INVOKABLE void insert(int row, const QString &text, const QVariant &keys);
    Q_INVOKABLE bool remove(int row);

public Q_SLOTS:
    void reload(const QString &id);

private:
    void save();
    
    QString m_remoteId;
};

#endif // REMOTEMACROMODEL_H
