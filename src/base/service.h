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

#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

class Service : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(ServiceType serviceType READ serviceType WRITE setServiceType
               NOTIFY serviceTypeChanged)

    Q_ENUMS(ServiceType)

public:
    enum ServiceType {
        Bouquet = 0,
        Channel,
        Recording,
        Unknown
    };
    
    explicit Service(QObject *parent = 0);
    explicit Service(const QString &id, const QString &title,
                     ServiceType serviceType, QObject *parent = 0);

    QString id() const;
    void setId(const QString &i);
    
    QString title() const;
    void setTitle(const QString &t);

    ServiceType serviceType() const;
    void setServiceType(ServiceType t);

Q_SIGNALS:
    void idChanged();
    void titleChanged();
    void serviceTypeChanged();

private:
    QString m_id;
    QString m_title;

    ServiceType m_serviceType;
};

#endif // SERVICE_H
