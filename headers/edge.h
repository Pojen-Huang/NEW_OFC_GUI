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


#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

using namespace ogdf;

class Node;

//! [0]
class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, int source_port, Node *destNode, int dest_port);

    Node *sourceNode() const;
    Node *destNode() const;
    
    node source_ogdf_node;
    node dest_ogdf_node;
    edge ogdf_edge;
    
    int sourcePortNum();
    int destPortNum();
    void setStatus(QString& s);

    void adjust();

    enum { Type = UserType + 2 };
    int type() const { return Type; }
    
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
private:
    Node *source, *dest;
    int s_port, d_port;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
    QString status;
};
//! [0]

#endif
