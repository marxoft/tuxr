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

#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QVariantMap>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

class Request : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)
    Q_PROPERTY(QVariantMap headers READ headers WRITE setHeaders RESET resetHeaders
               NOTIFY headersChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QByteArray result READ result NOTIFY finished)
    Q_PROPERTY(QString errorString READ errorString NOTIFY finished)

    Q_ENUMS(Status)

public:
    enum Status {
        Null = 0,
        Active,
        Canceled,
        Ready,
        Error
    };
    
    explicit Request(QObject *parent = 0);
    ~Request();

    QUrl url() const;

    QVariantMap headers() const;
    void setHeaders(const QVariantMap &h);
    void resetHeaders();

    Status status() const;
    
    QByteArray result() const;

    QString errorString() const;

    QNetworkAccessManager* networkAccessManager();
    void setNetworkAccessManager(QNetworkAccessManager *m);

public Q_SLOTS:
    void get(const QUrl &url);
    void post(const QString &data, const QUrl &url);
    void put(const QString &data, const QUrl &url);
    void deleteResource(const QUrl &url);
    
    void cancel();

protected:
    void setStatus(Status s);

    void setResult(const QByteArray &r);

    void setErrorString(const QString &e);

    virtual QNetworkRequest buildRequest(const QUrl &url) const;

protected Q_SLOTS:
    virtual void onReplyFinished();

Q_SIGNALS:
    void urlChanged();
    void headersChanged();
    void statusChanged(Status s);
    void finished();

private:
    QNetworkAccessManager *m_nam;
    QNetworkReply *m_reply;
    
    QVariantMap m_headers;

    Status m_status;

    QByteArray m_result;
    
    QString m_errorString;
};
    
#endif // REQUEST_H
