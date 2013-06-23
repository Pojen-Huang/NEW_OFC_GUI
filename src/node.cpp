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


#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QtGui>

#include "edge.h"
#include "node.h"
#include "graphwidget.h"

//! [0]

Node::Node(GraphWidget *graphWidget, NodeKind kind)
: graph(graphWidget) {
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    kind_value = kind;
    //qDebug("This is Node with kind :%i", kind);
}
//! [0]

QRectF Node::outlineRect(QString &myText) const {
    const int Padding = 2;
    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect(myText);
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
    rect.translate(-rect.center());
    return rect;
}

//! [1]

void Node::addEdge(Edge *edge) {
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const {
    return edgeList;
}
//! [1]

//! [2]

void Node::calculateForces() {
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }
    //! [2]

    //! [3]
    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;

    foreach(QGraphicsItem *item, scene()->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }
    //! [3]

    //! [4]
    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;

    foreach(Edge *edge, edgeList) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }
    //! [4]

    //! [5]
    if (qAbs(xvel) < 2.4 && qAbs(yvel) < 2.4) //0.1
        xvel = yvel = 0;
    //! [5]

    //! [6]
    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}
//! [6]

//! [7]

bool Node::advance() {
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
//! [7]

//! [8]

QRectF Node::boundingRect() const {
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
    // Add some extra space around the circle for easier touching with finger
    qreal adjust = 30;
    return QRectF(-10 - adjust, -10 - adjust,
            20 + adjust * 2, 20 + adjust * 2);
#else
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
            23 + adjust, 23 + adjust);
#endif
}
//! [8]

//! [9]

QPainterPath Node::shape() const {
    QPainterPath path;
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
    // Add some extra space around the circle for easier touching with finger
    path.addEllipse(-40, -40, 80, 80);
#else
    path.addEllipse(-10, -10, 20, 20);
#endif
    return path;
}
//! [9]

//! [10]

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);

    //qreal my_adjust = 2;
    //QRectF rect =  QRectF( -10 - my_adjust, -10 - my_adjust,
    //              17 + my_adjust, 17 + my_adjust);
    QRectF rect;
    QString myText;
    // add by Danny
    switch (kind_value) {
        case SwitchKind:
            painter->drawEllipse(-10, -10, 20, 20);
            break;
        case HostKind:
            painter->drawRoundRect(-10, -10, 20, 20);
            break;
        default:
            break;
    }

    QRadialGradient gradient(-3, -3, 10);

    if (option->state & QStyle::State_Sunken) {
        // the node (the widget) is sunken or pressed.
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::blue).light(120));
        gradient.setColorAt(0, QColor(Qt::darkBlue).light(120));

    } else {
        switch (kind_value) {
            case SwitchKind:
                gradient.setColorAt(0, Qt::yellow);
                gradient.setColorAt(1, Qt::darkYellow);
                break;
            case HostKind:
                gradient.setColorAt(0, Qt::cyan);
                gradient.setColorAt(1, Qt::darkCyan);
                break;
            default:
                gradient.setColorAt(0, Qt::red);
                gradient.setColorAt(1, Qt::darkRed);
                break;
        }
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));

    // add by Danny
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(4);
    painter->setFont(font);

    switch (kind_value) {
        case SwitchKind:
            painter->drawEllipse(-10, -10, 20, 20);
            myText = QString("Switch:") + this->id;
            rect = outlineRect(myText);
            painter->drawText(rect, Qt::AlignCenter, myText);
            break;
        case HostKind:
            painter->drawRoundRect(-10, -10, 20, 20);
            myText = QString("host") + this->id;
            rect = outlineRect(myText);
            painter->drawText(rect, Qt::AlignCenter, myText);
            break;
        default:
            break;
    }

    /* testing
    QPainterPath path;
    path.addRect(2, 2, 6, 6);

    path.moveTo(0, 0);
    path.cubicTo(9, 0,  5, 5,  9, 9);
    path.cubicTo(0, 9,  5, 5,  0, 0);

    painter->fillRect(0, 0, 10, 10, Qt::white);
    painter->setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine,
                     Qt::FlatCap, Qt::MiterJoin));
    painter->setBrush(QColor(122, 163, 39));

    painter->drawPath(path);
     */

}
//! [10]

//! [11]

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
    switch (change) {
        case ItemPositionHasChanged:
            foreach(Edge *edge, edgeList)
            edge->adjust();
            graph->itemMoved();
            break;
        default:
            break;
    };

    return QGraphicsItem::itemChange(change, value);
}
//! [11]

//! [12]

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
//! [12]
