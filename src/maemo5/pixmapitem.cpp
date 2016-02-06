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

#include "pixmapitem.h"
#include <QPainter>

PixmapItem::PixmapItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

QPixmap PixmapItem::pixmap() const {
    return m_pixmap;
}

void PixmapItem::setPixmap(const QPixmap &p) {
    m_pixmap = p;
    update();
    emit pixmapChanged();
}

void PixmapItem::resetPixmap() {
    setPixmap(QPixmap());
}

void PixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!m_pixmap.isNull()) {
        painter->drawPixmap(0, 0, m_pixmap.scaled(boundingRect().size().toSize(), Qt::IgnoreAspectRatio,
                            smooth() ? Qt::SmoothTransformation : Qt::FastTransformation));
    }
}
