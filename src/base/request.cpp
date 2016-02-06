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

#include "request.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#ifdef TUXR_DEBUG
#include <QDebug>
#endif

Request::Request(QObject *parent) :
    QObject(parent),
    m_nam(0),
    m_reply(0),
    m_status(Null)
{
}

Request::~Request() {
    if (m_reply) {
        m_reply->deleteLater();
        m_reply = 0;
    }
}

QUrl Request::url() const {
    return m_reply ? m_reply->url() : QUrl();
}

QVariantMap Request::headers() const {
    return m_headers;
}

void Request::setHeaders(const QVariantMap &h) {
    m_headers = h;
    emit headersChanged();
}

void Request::resetHeaders() {
    setHeaders(QVariantMap());
}

Request::Status Request::status() const {
    return m_status;
}

void Request::setStatus(Request::Status s) {
    if (s != status()) {
        m_status = s;
        emit statusChanged(s);
    }
}

QByteArray Request::result() const {
    return m_result;
}

void Request::setResult(const QByteArray &r) {
    m_result = r;
}

QString Request::errorString() const {
    return m_errorString;
}

void Request::setErrorString(const QString &e) {
    m_errorString = e;
}

QNetworkAccessManager* Request::networkAccessManager() {
    return m_nam ? m_nam : m_nam = new QNetworkAccessManager(this);
}

void Request::setNetworkAccessManager(QNetworkAccessManager *m) {
    if (m_nam) {
        m_nam->deleteLater();
    }

    m_nam = m;
}

void Request::get(const QUrl &url) {
    if (status() == Active) {
        return;
    }

    setStatus(Active);
    m_reply = networkAccessManager()->get(buildRequest(url));
    connect(m_reply, SIGNAL(finished()), SLOT(onReplyFinished()));
    emit urlChanged();
}

void Request::post(const QString &data, const QUrl &url) {
    if (status() == Active) {
        return;
    }

    setStatus(Active);
    m_reply = networkAccessManager()->post(buildRequest(url), data.toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(onReplyFinished()));
    emit urlChanged();
}

void Request::put(const QString &data, const QUrl &url) {
    if (status() == Active) {
        return;
    }

    setStatus(Active);
    m_reply = networkAccessManager()->put(buildRequest(url), data.toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(onReplyFinished()));
    emit urlChanged();
}

void Request::deleteResource(const QUrl &url) {
    if (status() == Active) {
        return;
    }

    setStatus(Active);
    m_reply = networkAccessManager()->deleteResource(buildRequest(url));
    emit urlChanged();
}

void Request::cancel() {
    if ((m_reply) && (m_reply->isRunning())) {
        m_reply->abort();
    }
}

QNetworkRequest Request::buildRequest(const QUrl &url) const {
#ifdef TUXR_DEBUG
    qDebug() << "Request::buildRequest: URL =" << url;
#endif
    QNetworkRequest request(url);
    QMapIterator<QString, QVariant> iterator(headers());

    while (iterator.hasNext()) {
        iterator.next();
#ifdef TUXR_DEBUG
        qDebug() << "Adding header:" << iterator.key() << "=" << iterator.value();
#endif
        request.setRawHeader(iterator.key().toUtf8(), iterator.value().toByteArray());
    }
    
    return request;
}

void Request::onReplyFinished() {
    if (!m_reply) {
        return;
    }

    setResult(m_reply->readAll());
    setErrorString(m_reply->errorString());

    switch (m_reply->error()) {
    case QNetworkReply::NoError:
        setStatus(Ready);
        break;
    case QNetworkReply::OperationCanceledError:
        setStatus(Canceled);
        break;
    default:
        setStatus(Error);
        break;
    }
    
    m_reply->deleteLater();
    m_reply = 0;
    emit finished();
}
