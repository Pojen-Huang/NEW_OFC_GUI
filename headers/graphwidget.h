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
