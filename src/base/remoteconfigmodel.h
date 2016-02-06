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

#ifndef REMOTECONFIGMODEL_H
#define REMOTECONFIGMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class RemoteConfigModel : public QAbstractListModel
{
    Q_OBJECT
    
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        RemoteTypeRole,
        NameRole,
        AddressRole,
        StreamingPortRole,
        UsernameRole,
        PasswordRole,
        MacrosRole
    };

    explicit RemoteConfigModel(QObject *parent = 0);
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE QVariant data(int row, const QByteArray &role) const;

    bool setData(const QModelIndex &index, const QVariant &data, int role);
    Q_INVOKABLE bool setData(int row, const QVariant &data, const QByteArray &role);

    Q_INVOKABLE QVariant headerData(int section = 0, Qt::Orientation orientation = Qt::Vertical,
                                    int role = Qt::DisplayRole) const;

    QMap<int, QVariant> itemData(const QModelIndex &index) const;
    Q_INVOKABLE QVariantMap itemData(int row) const;

    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &data);
    Q_INVOKABLE bool setItemData(int row, const QVariantMap &data);
    
    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = 1,
                          Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;
    Q_INVOKABLE virtual int match(int start, const QByteArray &role, const QVariant &value,
                                  int flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;

public Q_SLOTS:
    void reload();
    void clear();

private Q_SLOTS:
    void onRemoteAdded(const QString &id);
    void onRemoteChanged(const QString &id);
    void onRemoteRemoved(const QString &id);

Q_SIGNALS:
    void countChanged(int c);

private:
    QStringList m_remoteIds;

    QHash<int, QByteArray> m_roles;
};

#endif // REMOTECONFIGMODEL_H
