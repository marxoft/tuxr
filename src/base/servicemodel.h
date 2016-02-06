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

#ifndef SERVICEMODEL_H
#define SERVICEMODEL_H

#include "servicelist.h"
#include <QStandardItemModel>

class ServiceModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY(ServiceList *services READ services WRITE setServices NOTIFY servicesChanged)
    Q_PROPERTY(Request::Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY statusChanged)
    
    Q_ENUMS(Request::Status)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ServiceTypeRole,
        SectionRole
    };
    
    explicit ServiceModel(QObject *parent = 0);

    ServiceList* services() const;
    void setServices(ServiceList *s);
    
    Request::Status status() const;
    
    QString errorString() const;
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif
    
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE virtual QVariant data(int row, const QVariant &parentIndex, const QByteArray &role) const;

    bool setData(const QModelIndex &index, const QVariant &data, int role);
    Q_INVOKABLE virtual bool setData(int row, const QVariant &parentIndex, const QVariant &data,
                                     const QByteArray &role);

    Q_INVOKABLE QVariant headerData(int section = 0, Qt::Orientation orientation = Qt::Horizontal,
                                    int role = Qt::DisplayRole) const;

    QMap<int, QVariant> itemData(const QModelIndex &parent) const;
    Q_INVOKABLE virtual QVariantMap itemData(int row, const QVariant &parentIndex) const;

    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &data);
    Q_INVOKABLE virtual bool setItemData(int row, const QVariant &parentIndex, const QVariantMap &data);
    
    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = 1,
                          Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;
    Q_INVOKABLE virtual int match(int start, const QVariant &parentIndex, const QByteArray &role, const QVariant &value,
                                  int flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;

public Q_SLOTS:    
    void reload(const QModelIndex &parent);
    void reload(const QVariant &parentIndex);
    void cancel();

protected Q_SLOTS:
    void onFinished();

Q_SIGNALS:
    void servicesChanged();
    void statusChanged(Request::Status s);

private:
    ServiceList *m_services;

    QPersistentModelIndex m_parent;
    
    QHash<int, QByteArray> m_roles;
};

Q_DECLARE_METATYPE(QModelIndex)

#endif // SERVICEMODEL_H
