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


#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QtGui>
#include <QtGui/QGraphicsView>
//#include <nzmqt/DataModels.h>

#include "edge.h"
#include "node.h"
#include "annotation.h"
//#include "nzmqt/DataModels.h"


class Node;
class Edge;

enum LayoutAlgorithms {
    PlanarStraightLA,
    PlanarDrawLA,
    MixedModelLA,
    UpwardPlanarizationLA
};

class SwitchCluster {
public:
    SwitchCluster(QString &id);
    ~SwitchCluster();
    
    QString id;
    QList<QString> *switches;
    QList<Node*> *nodes;
    QList<Edge*> *edges;
    QGraphicsScene *scene;
};

//! [0]
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
    
    void itemMoved();
    bool compareLinkStatus();
    void setForceCalculation(bool flag);
    void setLayout(enum LayoutAlgorithms la);
    void setZeroMQConn(QString &reqrepConn, QString &pubsub_conn);
    void addSwitchCluster(QString &cluster, QString &sw);
    SwitchCluster *findSwitchCluster(QString &dpid);
    void deleteSwitchCluster();
    void setCurrentCluster(QString &cluster);
    void resetSwitchCluster();

public slots:
    void requestTopology();
    void shuffle();
    void zoomIn();
    void zoomOut();
    void updateDataModel(const QList<QByteArray>& reply);
    void updateDataModel(QByteArray& result);
    void updateHostDataModel(QByteArray& result);
    void updateScene();
    void abortRequest();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);
    Node *findNode(QString& id, enum NodeKind type);
    Node *findHostNode(SwitchCluster *sc, QString& id, enum NodeKind type);
    Edge *findEdge(Node* from_node, int from_port, Node* to_node, int to_port);

private:
    Node *createNode(SwitchCluster *sc, QString& id, enum NodeKind type);
    Edge *createEdge(SwitchCluster *sc, Node* from_node, int from_port, 
    Node* to_node, int to_port);
    
    QString address;
    int timerId;
    Node *centerNode;
    QMap<QString, SwitchCluster*> *cluster_qmap;
    SwitchCluster *curr_cluster;
    
    /* for topology setting */
    bool force_calculation_flag;
    LayoutAlgorithms layout;
    
};
//! [0]

#endif
