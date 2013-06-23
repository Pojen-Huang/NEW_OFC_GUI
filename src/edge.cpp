        /****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

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

