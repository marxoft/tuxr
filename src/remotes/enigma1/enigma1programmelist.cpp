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

#include "enigma1programmelist.h"
#include "programme.h"
#include <QStringList>
#include <QXmlStreamReader>

Enigma1ProgrammeList::Enigma1ProgrammeList(QObject *parent) :
    ProgrammeList(parent),
    m_request(0),
    m_status(Request::Null)
{
}

QString Enigma1ProgrammeList::address() const {
    return m_address;
}

void Enigma1ProgrammeList::setAddress(const QString &a) {
    if (a != address()) {
        m_address = a;
        emit addressChanged();
    }
}

QString Enigma1ProgrammeList::username() const {
    return m_username;
}

void Enigma1ProgrammeList::setUsername(const QString &u) {
    if (u != username()) {
        m_username = u;
        emit usernameChanged();
    }
}

QString Enigma1ProgrammeList::password() const {
    return m_password;
}

void Enigma1ProgrammeList::setPassword(const QString &p) {
    if (p != password()) {
        m_password = p;
        emit passwordChanged();
    }
}

QString Enigma1ProgrammeList::errorString() const {
    return m_errorString;
}

void Enigma1ProgrammeList::setErrorString(const QString &e) {
    m_errorString = e;
}

Request::Status Enigma1ProgrammeList::status() const {
    return m_status;
}

void Enigma1ProgrammeList::setStatus(Request::Status s) {
    if (s != status()) {
        m_status = s;
        emit statusChanged(s);
    }
}

int Enigma1ProgrammeList::count() const {
    return m_programmes.size();
}

Programme* Enigma1ProgrammeList::at(int i) const {
    if ((i >= 0) && (i < m_programmes.size())) {
        return m_programmes.at(i);
    }

    return 0;
}

void Enigma1ProgrammeList::clear() {
    if (!m_programmes.isEmpty()) {
        qDeleteAll(m_programmes);
        m_programmes.clear();
        emit countChanged(0);
    }
}

void Enigma1ProgrammeList::reload(const QString &ref) {
    setStatus(Request::Active);
    
    if (address().isEmpty()) {
        setErrorString(tr("IP address is empty."));
        setStatus(Request::Error);
        emit finished();
        return;
    }

    clear();
    
    if (!m_request) {
        m_request = new Request(this);
        connect(m_request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    }
    
    if ((!username().isEmpty()) && (!password().isEmpty())) {
        QVariantMap headers;
        headers["Authorization"] = "Basic " + QString("%1:%2").arg(username())
                                                     .arg(password()).toUtf8().toBase64();
        m_request->setHeaders(headers);
    }

    if (ref.section("/", 0, 0).endsWith(":0:0:0:0:0:")) {
        // Load bouquet EPG
        m_request->get(QUrl::fromUserInput(address() + "/getMultiEPG?ref=" + ref));
    }
    else {
        // Load channel EPG
        m_request->get(QUrl::fromUserInput(address() + "/getcurrentepg?type=extended&ref="
                                           + ref));
    }
}

void Enigma1ProgrammeList::cancel() {
    if (m_request) {
        m_request->cancel();
    }
}

void Enigma1ProgrammeList::onRequestFinished() {
    if (!m_request) {
        return;
    }

    if (m_request->status() == Request::Ready) {
	QString result = QString::fromUtf8(m_request->result());
	result = result.mid(result.indexOf("<tbody>"));
	result = result.left(result.indexOf("</tbody>") + 8);
        result.remove(QRegExp("<img [^>]*>"));
        result.remove("&nbsp;");
        result.remove("<br>");
        result.replace(" & ", " &amp; ");
        result.replace(" ~ ", " &amp; ");
        QXmlStreamReader reader(result);

        while (!reader.atEnd()) {
	    reader.readNextStartElement();
            
	    if (reader.name() == "a") {
                QString href = reader.attributes().value("href").toString();

                if (href.startsWith("javascript:record")) {
                    href = href.mid(href.indexOf("(") + 1);
                    href.chop(1);
                    href.remove("'");
		    const QStringList list = href.split(",", QString::SkipEmptyParts);
			    
                    if (list.size() > 4) {
                        Programme *programme = new Programme(this);
                        programme->setId(list.first());
                        programme->setTitle(list.at(3));
                        programme->setStartTime(QDateTime::fromTime_t(list.at(1).toUInt()));
                        programme->setEndTime(programme->startTime().addSecs(list.at(2).toInt()));
                        programme->setChannelId(list.first());
                        programme->setChannelTitle(list.at(4));

                        reader.readNextStartElement();

                        while (!reader.atEnd()) {
                            if ((reader.name() == "span")
                                && (reader.attributes().value("class") == "description")) {
                                programme->setDescription(reader.readElementText());
                                break;
                            }

                            reader.readNextStartElement();
                        }

                        m_programmes << programme;
                    }
                }
            }
        }
        
        emit countChanged(count());
    }

    setErrorString(m_request->errorString());
    setStatus(m_request->status());
    emit finished();
}
