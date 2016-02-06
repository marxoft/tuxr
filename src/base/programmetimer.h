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

#ifndef PROGRAMMETIMER_H
#define PROGRAMMETIMER_H

#include <QObject>
#include <QDateTime>

class ProgrammeTimer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime NOTIFY timeChanged)
    Q_PROPERTY(QDateTime endTime READ endTime WRITE setEndTime NOTIFY timeChanged)
    Q_PROPERTY(int duration READ duration NOTIFY timeChanged)
    Q_PROPERTY(QString channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)
    Q_PROPERTY(QString channelTitle READ channelTitle WRITE setChannelTitle NOTIFY channelTitleChanged)
    Q_PROPERTY(EventType eventType READ eventType WRITE setEventType NOTIFY eventTypeChanged)
    Q_PROPERTY(Days recurringDays READ recurringDays WRITE setRecurringDays RESET resetRecurringDays
               NOTIFY recurringDaysChanged)
    Q_PROPERTY(Action action READ action WRITE setAction NOTIFY actionChanged)
    Q_PROPERTY(Action afterAction READ afterAction WRITE setAfterAction NOTIFY afterActionChanged)

    Q_ENUMS(Action Day EventType)
    Q_FLAGS(Days)

public:
    enum Action {
        None = 0,
        Dvr,
        Ngrab,
        Zap,
        Standby
    };
    
    enum Day {
        Monday = 0x0001,
        Tuesday = 0x0002,
        Wednesday = 0x0004,
        Thursday = 0x0008,
        Friday = 0x0010,
        Saturday = 0x0020,
        Sunday = 0x0040
    };

    enum EventType {
        OnetimeEvent = 0,
        RecurringEvent
    };

    Q_DECLARE_FLAGS(Days, Day)
    
    explicit ProgrammeTimer(QObject *parent = 0);
    explicit ProgrammeTimer(const QString &id, const QString &description,
                            const QDateTime &startTime, const QDateTime &endTime,
                            const QString &channelId, const QString &channelTitle,
                            EventType eventType, Days recurringDays, Action action,
                            Action afterAction, QObject *parent = 0);

    QString id() const;
    void setId(const QString &i);
    
    QString description() const;
    void setDescription(const QString &d);

    QDateTime startTime() const;
    void setStartTime(const QDateTime &dt);
    
    QDateTime endTime() const;
    void setEndTime(const QDateTime &dt);

    int duration() const;

    QString channelId() const;
    void setChannelId(const QString &i);
    
    QString channelTitle() const;
    void setChannelTitle(const QString &t);

    EventType eventType() const;
    void setEventType(EventType t);

    Days recurringDays() const;
    void setRecurringDays(Days d);
    void resetRecurringDays();
    Q_INVOKABLE void setRecurringDay(int d, bool enabled = true);
    Q_INVOKABLE bool testRecurringDay(int d) const;

    Action action() const;
    void setAction(Action a);

    Action afterAction() const;
    void setAfterAction(Action a);

Q_SIGNALS:
    void idChanged();
    void descriptionChanged();
    void timeChanged();
    void channelIdChanged();
    void channelTitleChanged();
    void eventTypeChanged();
    void recurringDaysChanged();
    void actionChanged();
    void afterActionChanged();

private:
    QString m_id;
    QString m_description;

    QDateTime m_startTime;
    QDateTime m_endTime;

    QString m_channelId;
    QString m_channelTitle;

    EventType m_eventType;

    Days m_recurringDays;
    
    Action m_action;
    Action m_afterAction;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ProgrammeTimer::Days)

#endif // PROGRAMMETIMER_H
