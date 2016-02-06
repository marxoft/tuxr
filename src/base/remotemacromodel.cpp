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

#include "remotemacromodel.h"
#include <QSettings>

RemoteMacroModel::RemoteMacroModel(QObject *parent) :
    SelectionModel(parent)
{
}

bool RemoteMacroModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (SelectionModel::setData(index, value, role)) {
        if (!m_remoteId.isEmpty()) {
            save();
        }
        
        return true;
    }
    
    return false;
}

void RemoteMacroModel::append(const QString &text, const QVariant &keys) {
    SelectionModel::append(text, keys);
    
    if (!m_remoteId.isEmpty()) {
        save();
    }
}

void RemoteMacroModel::insert(int row, const QString &text, const QVariant &keys) {
    SelectionModel::insert(row, text, keys);
    
    if (!m_remoteId.isEmpty()) {
        save();
    }
}

bool RemoteMacroModel::remove(int row) {
    if (SelectionModel::remove(row)) {
        if (!m_remoteId.isEmpty()) {
            save();
        }
        
        return true;
    }
    
    return false;
}

void RemoteMacroModel::reload(const QString &id) {
    m_remoteId = id;
    clear();
    QSettings settings("TuxR", "Remotes");
    settings.beginGroup(id);
    const int size = settings.beginReadArray("macros");
    
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        SelectionModel::append(settings.value("text").toString(), settings.value("keys"));
    }
    
    settings.endArray();
    settings.endGroup();
}

void RemoteMacroModel::save() {
    QSettings settings("TuxR", "Remotes");
    settings.beginGroup(m_remoteId);
    settings.beginWriteArray("macros");
    
    for (int i = 0; i < rowCount(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("text", data(i, "name"));
        settings.setValue("keys", data(i, "value"));
    }
    
    settings.endArray();
    settings.endGroup();
}
