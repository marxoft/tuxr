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

#include "screenshotdialog.h"
#include "instantiator.h"
#include "screenshotimagetypemodel.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

ScreenShotDialog::ScreenShotDialog(const QString &remoteId, QWidget *parent) :
    QDialog(parent),
    m_screenShot(Instantiator::createScreenShot(remoteId, this)),
    m_imageTypeModel(new ScreenShotImageTypeModel(this)),
    m_label(new QLabel(this)),
    m_imageTypeSelector(new QComboBox(this)),
    m_reloadButton(new QPushButton(tr("Reload"), this)),
    m_saveButton(new QPushButton(tr("Save"), this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, this)),
    m_layout(new QGridLayout(this))
{
    setWindowTitle(tr("Screenshot"));

    m_label->setFixedSize(QSize(640, 360));
    m_label->setScaledContents(true);

    m_imageTypeSelector->setModel(m_imageTypeModel);

    m_reloadButton->setShortcut(QKeySequence::Refresh);
    m_saveButton->setShortcut(QKeySequence::Save);
    
    m_layout->addWidget(m_label, 0, 0, 1, 5);
    m_layout->addWidget(new QLabel(tr("Screenshot type:"), this), 1, 0);
    m_layout->addWidget(m_imageTypeSelector, 1, 1);
    m_layout->addWidget(m_reloadButton, 1, 2);
    m_layout->addWidget(m_saveButton, 1, 3);
    m_layout->addWidget(m_buttonBox, 1, 4);

    connect(m_screenShot, SIGNAL(finished()), this, SLOT(onScreenShotFinished()));
    connect(m_reloadButton, SIGNAL(clicked()), this, SLOT(reload()));
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

ScreenShot::ImageType ScreenShotDialog::imageType() const {
    return ScreenShot::ImageType(m_imageTypeSelector->itemData(
                                 m_imageTypeSelector->currentIndex()).toInt());
}

void ScreenShotDialog::setImageType(ScreenShot::ImageType type) {
    m_imageTypeSelector->setCurrentIndex(m_imageTypeSelector->findData(type));
}

void ScreenShotDialog::reload() {
    m_screenShot->reload(imageType());
}

void ScreenShotDialog::reload(ScreenShot::ImageType type) {
    setImageType(type);
    reload();
}

void ScreenShotDialog::save() {
    const QString fileName = QFileDialog::getSaveFileName(this);

    if (!fileName.isEmpty()) {
        if (!m_screenShot->save(fileName)) {
            QMessageBox::critical(this, tr("Error"), tr("Unable to save image to file."));
        }
    }
}

void ScreenShotDialog::onScreenShotFinished() {
    switch (m_screenShot->status()) {
    case Request::Ready:
        m_label->setPixmap(m_screenShot->pixmap());
        break;
    case Request::Error:
        QMessageBox::critical(this, tr("Error"), m_screenShot->errorString());
        break;
    default:
        break;
    }
}
