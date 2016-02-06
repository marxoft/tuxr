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

#include "remoteview.h"
#include "instantiator.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QStringList>

RemoteView::RemoteView(const QString &remoteId, QWidget *parent) :
    QWidget(parent),
    m_remote(Instantiator::createRemote(remoteId, this)),
    m_layout(new QGridLayout(this))
{
    addButton(tr("Power"), QString(), Remote::Key_Power, 0, 1);
    
    addButton("1", QString(), Remote::Key_1, 1, 0);
    addButton("2", QString(), Remote::Key_2, 1, 1);
    addButton("3", QString(), Remote::Key_3, 1, 2);
    addButton("4", QString(), Remote::Key_4, 2, 0);
    addButton("5", QString(), Remote::Key_5, 2, 1);
    addButton("6", QString(), Remote::Key_6, 2, 2);
    addButton("7", QString(), Remote::Key_7, 3, 0);
    addButton("8", QString(), Remote::Key_8, 3, 1);
    addButton("9", QString(), Remote::Key_9, 3, 2);
    addButton("<", QString(), Remote::Key_Back, 4, 0);
    addButton("0", QString(), Remote::Key_0, 4, 1);
    addButton(">", QString(), Remote::Key_Forward, 4, 2);

    addButton(tr("Volume+"), QString(), Remote::Key_VolumeUp, 0, 3);
    addButton(tr("Mute"), QString(), Remote::Key_VolumeMute, 0, 4);
    addButton(tr("Bouquet+"), QString(), Remote::Key_BouquetUp, 0, 5);
    addButton(tr("Volume-"), QString(), Remote::Key_VolumeDown, 1, 3);
    addButton(tr("Exit"), QString(), Remote::Key_VolumeMute, 1, 4);
    addButton(tr("Bouquet-"), QString(), Remote::Key_BouquetDown, 1, 5);
    
    addButton(tr("Info"), QString(), Remote::Key_Info, 2, 3);
    addButton(tr("Up"), QString(), Remote::Key_Up, 2, 4);
    addButton(tr("Menu"), QString(), Remote::Key_Menu, 2, 5);
    addButton(tr("Left"), QString(), Remote::Key_Left, 3, 3);
    addButton(tr("OK"), QString(), Remote::Key_Enter, 3, 4);
    addButton(tr("Right"), QString(), Remote::Key_Right, 3, 5);
    addButton(tr("Audio"), QString(), Remote::Key_Audio, 4, 3);
    addButton(tr("Down"), QString(), Remote::Key_Down, 4, 4);
    addButton(tr("Video"), QString(), Remote::Key_Video, 4, 5);
    
    addButton(tr("Red"), QString(), Remote::Key_Red, 5, 1);
    addButton(tr("Green"), QString(), Remote::Key_Green, 5, 2);
    addButton(tr("Yellow"), QString(), Remote::Key_Yellow, 5, 3);
    addButton(tr("Blue"), QString(), Remote::Key_Green, 5, 4);

    addButton(tr("TV"), QString(), Remote::Key_Tv, 6, 1);
    addButton(tr("Radio"), QString(), Remote::Key_Radio, 6, 2);
    addButton(tr("Text"), QString(), Remote::Key_Text, 6, 3);
    addButton(tr("Help"), QString(), Remote::Key_Help, 6, 4);

    connect(m_remote, SIGNAL(finished()), this, SLOT(onRemoteFinished()));
}

void RemoteView::addButton(const QString &text, const QString &icon, Remote::Key key, int row, int column) {
    QPushButton *button;
    
    if (!icon.isEmpty()) {
        button = new QPushButton(icon.contains("/") ? QIcon(icon) : QIcon::fromTheme(icon),
                                 text, this);
    }
    else {
        button = new QPushButton(text, this);
    }

    button->setProperty("key", key);
    m_layout->addWidget(button, row, column);

    if (m_remote->keyIsEnabled(key)) {
        connect(button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    }
    else {
        button->setEnabled(false);
    }
}

void RemoteView::onButtonClicked() {
    if (const QObject *obj = sender()) {
        m_remote->sendKey(obj->property("key").toInt());
    }
}

void RemoteView::onRemoteFinished() {
    if (m_remote->status() == Request::Error) {
        QMessageBox::critical(this, tr("Error"), m_remote->errorString());
    }
}
