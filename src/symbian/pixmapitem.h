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

#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QDeclarativeItem>
#include <QPixmap>

class PixmapItem : public QDeclarativeItem
{
    Q_OBJECT
    
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap RESET resetPixmap NOTIFY pixmapChanged)

public:
    explicit PixmapItem(QDeclarativeItem *parent = 0);
    
    QPixmap pixmap() const;
    void setPixmap(const QPixmap &p);
    void resetPixmap();
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

Q_SIGNALS:
    void pixmapChanged();

private:
    QPixmap m_pixmap;
};

#endif // IMAGELABEL_H
