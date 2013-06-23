/*
 * Author: TeYen Liu 
 *
 * Copyright (C) 2013 TeYen Liu
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */



#include <QPainter>

#include "edge.h"
#include "node.h"

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

//! [0]
Edge::Edge(Node *sourceNode, int source_port, Node *destNode, int dest_port)
    : arrowSize(10)
{
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    s_port = source_port;
    dest->addEdge(this);
    d_port = dest_port;
    status = "unknown"; // Default value
    
    adjust();
}
//! [0]

//! [1]
Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

int Edge::sourcePortNum()
{
    return s_port;
}

int Edge::destPortNum()
{
    return d_port;
}

void Edge::setStatus(QString& s) 
{
    status = s;
}

//! [1]

//! [2]
void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}
//! [2]

//! [3]
QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [3]

//! [4]
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
//! [4]

//! [5]
    // Draw the line itself
    if (status.compare("up") == 0) {
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else if (status.compare("down") == 0) {
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    } else {
        // unknown
        painter->setPen(QPen(Qt::black, 1, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawLine(line);
//! [5]

//! [6]
    // Draw the arrows
    /*
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                  cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                  cos(angle + Pi - Pi / 3) * arrowSize);   
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
     */
//! [6]
    
//! [7]
    // Draw the port number
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(4);
    painter->setFont(font);
    
    const int Padding = 4;
    QString spText = QString::number(s_port);
    QString dpText = QString::number(d_port);
    QFontMetricsF metricsS = painter->font();
    QFontMetricsF metricsD = painter->font();
    
    QRectF rectS = metricsS.boundingRect(spText);
    QRectF rectD = metricsD.boundingRect(dpText);
    
    int sx = sourcePoint.x(), sy = sourcePoint.y(), dx = destPoint.x(), dy = destPoint.y();
    
    if ((sx > dx) && (sy > dy)) {    
        rectS.adjust(-Padding + sx, -Padding + sy, 
            - Padding + sx, -Padding + sy);
        
        rectD.adjust(Padding + dx, Padding + dy, 
             Padding + dx, Padding + dy);
    } else if ((sx > dx) && (sy < dy)) {
        
        rectS.adjust(-Padding + sx, Padding + sy, 
            - Padding + sx, Padding + sy);
        
        rectD.adjust(Padding + dx, -Padding + dy, 
             Padding + dx, -Padding + dy);
    } else if ((sx < dx) && (sy > dy)) {
        rectS.adjust(Padding + sx, -Padding + sy, 
            Padding + sx, -Padding + sy);
        
        rectD.adjust(-Padding + dx, Padding + dy, 
             -Padding + dx, Padding + dy);
    } else {
        rectS.adjust(Padding + sx, Padding + sy, 
            Padding + sx, Padding + sy);
        
        rectD.adjust(-Padding + dx, -Padding + dy, 
             -Padding + dx, -Padding + dy);
    }
    
    painter->drawText(rectS, Qt::AlignAbsolute, spText);
    painter->drawText(rectD, Qt::AlignAbsolute, dpText);
//! [7]
    
}

