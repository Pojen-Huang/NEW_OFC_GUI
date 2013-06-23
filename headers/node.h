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


#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

        using namespace ogdf;

//! [0]

enum NodeKind {
    SwitchKind, HostKind
};
    

class Node : public QGraphicsItem {
public:

    Node(GraphWidget *graphWidget, NodeKind kind);
    QString id;
    /* Give a reference of ogdf node */
    node ogdf_node;

    QRectF outlineRect(QString &myText) const;

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum {
        Type = UserType + 1
    };

    int type() const {
        return Type;
    }

    void calculateForces();
    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    int kind_value;
};
//! [0]

#endif
