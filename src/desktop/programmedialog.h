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

#ifndef PROGRAMMEDIALOG_H
#define PROGRAMMEDIALOG_H

#include <QDialog>
#include <QDateTime>

class ProgrammeView;
class QDialogButtonBox;
class QVBoxLayout;

class ProgrammeDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime)
    Q_PROPERTY(QDateTime endTime READ endTime WRITE setEndTime)
    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(QString channelTitle READ channelTitle WRITE setChannelTitle)

public:
    explicit ProgrammeDialog(QWidget *parent = 0);

    QString title() const;
    void setTitle(const QString &t);

    QString description() const;
    void setDescription(const QString &d);

    QDateTime startTime() const;
    void setStartTime(const QDateTime &dt);

    QDateTime endTime() const;
    void setEndTime(const QDateTime &dt);

    int duration() const;
    
    QString channelTitle() const;
    void setChannelTitle(const QString &t);
    
private:
    ProgrammeView *m_view;

    QDialogButtonBox *m_buttonBox;

    QVBoxLayout *m_layout;
};

#endif // PROGRAMMEDIALOG_H
