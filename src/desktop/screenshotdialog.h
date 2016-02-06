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

#ifndef SCREENSHOTDIALOG_H
#define SCREENSHOTDIALOG_H

#include "screenshot.h"
#include <QDialog>

class ScreenShotImageTypeModel;
class QComboBox;
class QDialogButtonBox;
class QGridLayout;
class QLabel;
class QPushButton;

class ScreenShotDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(ScreenShot::ImageType imageType READ imageType WRITE setImageType)

public:
    explicit ScreenShotDialog(const QString &remoteId, QWidget *parent = 0);

    ScreenShot::ImageType imageType() const;
    void setImageType(ScreenShot::ImageType type);

public Q_SLOTS:
    void reload();
    void reload(ScreenShot::ImageType type);
    
private Q_SLOTS:
    void save();
        
    void onScreenShotFinished();

private:
    ScreenShot *m_screenShot;
    ScreenShotImageTypeModel *m_imageTypeModel;

    QLabel *m_label;

    QComboBox *m_imageTypeSelector;

    QPushButton *m_reloadButton;
    QPushButton *m_saveButton;

    QDialogButtonBox *m_buttonBox;

    QGridLayout *m_layout;
};

#endif // SCREENSHOTDIALOG_H
