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
#include <math.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/planarlayout/PlanarStraightLayout.h>
#include <ogdf/planarlayout/PlanarDrawLayout.h>
#include <ogdf/planarlayout/MixedModelLayout.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/upward/UpwardPlanarizationLayout.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>

#include "nzmqt/reqrep/ReqRepClient.h"
#include "graphwidget.h"

using namespace ogdf;

/*** initialize ogdf object ***/
PlanarStraightLayout psl;
PlanarDrawLayout pdl;
MixedModelLayout mml;
UpwardPlanarizationLayout upl;
Graph G;
GraphAttributes GA(G, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics | ogdf::GraphAttributes::nodeLabel);
/******************************/
nzmqt::ReqRepClient *rrclient;

//! [0]

SwitchCluster::SwitchCluster(QString &id) {
    this->id = id;
    switches = new QList<QString > ();
    nodes = new QList<Node*>();
    edges = new QList<Edge*>();
    scene = new QGraphicsScene();
}

GraphWidget::GraphWidget(QWidget *parent)
: QGraphicsView(parent), timerId(0) {
    /* Default layout algorithm */
    layout = PlanarStraightLA;

    force_calculation_flag = true;
    cluster_qmap = new QMap<QString, SwitchCluster*>();

    QGraphicsScene *default_scene = new QGraphicsScene(this);
    default_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    default_scene->setSceneRect(-20, -20, 400, 400);
    this->setScene(default_scene);

    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    /* init scale ratio value */
    scale(qreal(1.2), qreal(1.2));
    setMinimumSize(240, 240);

    //! [0]

    //! [1]
    /* Original
    Node *node1 = new Node(this, Node::Switch);
    Node *node2 = new Node(this, Node::Switch);
    Node *node3 = new Node(this, Node::Switch);
    Node *node4 = new Node(this, Node::Switch);
    centerNode = new Node(this, Node::Switch);
    Node *node6 = new Node(this, Node::Switch);
    Node *node7 = new Node(this, Node::Switch);
    Node *node8 = new Node(this, Node::Switch);
    Node *node9 = new Node(this, Node::Switch);
    scene->addItem(node1);
    scene->addItem(node2);
    scene->addItem(node3);
    scene->addItem(node4);
    scene->addItem(centerNode);
    scene->addItem(node6);
    scene->addItem(node7);
    scene->addItem(node8);
    scene->addItem(node9);
    scene->addItem(new Edge(node1, node2));
    scene->addItem(new Edge(node2, node3));
    scene->addItem(new Edge(node2, centerNode));
    scene->addItem(new Edge(node3, node6));
    scene->addItem(new Edge(node4, node1));
    scene->addItem(new Edge(node4, centerNode));
    scene->addItem(new Edge(centerNode, node6));
    scene->addItem(new Edge(centerNode, node8));
    scene->addItem(new Edge(node6, node9));
    scene->addItem(new Edge(node7, node4));
    scene->addItem(new Edge(node8, node7));
    scene->addItem(new Edge(node9, node8));

    node1->setPos(-50, -50);
    node2->setPos(0, -50);
    node3->setPos(50, -50);
    node4->setPos(-50, 0);
    centerNode->setPos(0, 0);
    node6->setPos(50, 0);
    node7->setPos(-50, 50);
    node8->setPos(0, 50);
    node9->setPos(50, 50);
     */

    // annotation testing
    //Annotation *annotation = new Annotation("switch1", true);
    //annotation->setPos(centerNode->pos());
    //scene->addItem(annotation);
}

SwitchCluster *GraphWidget::findSwitchCluster(QString &sw) {

    if (cluster_qmap != NULL) {
        QList<SwitchCluster*> values = cluster_qmap->values();

        foreach(SwitchCluster *value, values) {
            if (value->switches->contains(sw)) {
                return value;
            }
        }
    }
    return NULL;
}

void GraphWidget::addSwitchCluster(QString &cluster, QString &sw) {

    SwitchCluster *sc = NULL;
    QMapIterator<QString, SwitchCluster*> iter(*cluster_qmap);
    while (iter.hasNext()) {
        iter.next();
        if (iter.key() == cluster) {
            sc = iter.value();
        }
    }
    if (sc == NULL) {
        sc = new SwitchCluster(cluster);
        cluster_qmap->insert(cluster, sc);
        qDebug() << "cluster_qmap inserts " << cluster;
    }
    sc->switches->append(sw);
    qDebug() << "switch appends " << sw;
}

void GraphWidget::deleteSwitchCluster() {
    if (cluster_qmap != NULL) {
        cluster_qmap->clear();
    }
}

void GraphWidget::resetSwitchCluster() {
    SwitchCluster *sc = NULL;
    QMapIterator<QString, SwitchCluster*> iter(*cluster_qmap);
    while (iter.hasNext()) {
        iter.next();
        sc = iter.value();
        sc->nodes->clear();
        sc->edges->clear();
        sc->scene->clear();
    }
}

void GraphWidget::setCurrentCluster(QString &cluster) {

    ////// Testing ///////
    qDebug() << "Count:" << cluster_qmap->count();
    QList<SwitchCluster*> values = cluster_qmap->values();

    foreach(SwitchCluster *value, values) {
        qDebug() << value->id << ":" << value->switches->at(0);
    }

    SwitchCluster *sc = NULL;
    QMapIterator<QString, SwitchCluster*> iter(*cluster_qmap);
    while (iter.hasNext()) {
        iter.next();
        if (iter.key() == cluster) {
            curr_cluster = iter.value();
            return;
        }
    }
    qDebug() << "Cannot find cluster in QMap!!";
}

void GraphWidget::setZeroMQConn(QString &reqrepConn, QString &pubsub_conn) {
    /* Initialize variables */
    rrclient = new nzmqt::ReqRepClient(reqrepConn, (QObject*)this);
}

void GraphWidget::setForceCalculation(bool fc_flag) {
    qDebug("set Force Calculation:%i", fc_flag);
    force_calculation_flag = fc_flag;
}

void GraphWidget::setLayout(enum LayoutAlgorithms la) {
    layout = la;
}

Node *GraphWidget::createNode(SwitchCluster *sc, QString& id, enum NodeKind type) {
    Node *n = new Node(this, type);
    n->id = id;
    sc->nodes->append(n);
    return n;
}

Node *GraphWidget::findNode(QString& id, enum NodeKind type) {
    //Check which cluster that the node is belong to
    SwitchCluster *sc = findSwitchCluster(id);

    Node* n = NULL;
    for (int i = 0; i < sc->nodes->count(); i++) {
        n = sc->nodes->at(i);
        if (QString::compare(n->id, id) == 0) {
            return n;
        }
    }
    /* if finding nothing, just create a new one */
    n = createNode(sc, id, type);
    sc->scene->addItem(n);
    /* ogdf */
    node ogdf_n = G.newNode();
    n->ogdf_node = ogdf_n;

    return n;
}

Node *GraphWidget::findHostNode(SwitchCluster *sc, QString& id, enum NodeKind type) {
    Node* n = NULL;
    for (int i = 0; i < sc->nodes->count(); i++) {
        n = sc->nodes->at(i);
        if (QString::compare(n->id, id) == 0) {
            return n;
        }
    }
    /* if finding nothing, just create a new one */
    n = createNode(sc, id, type);
    sc->scene->addItem(n);
    /* ogdf */
    node ogdf_n = G.newNode();
    n->ogdf_node = ogdf_n;

    return n;
}

Edge *GraphWidget::createEdge(SwitchCluster *sc, Node* from_node, int from_port, Node* to_node, int to_port) {
    Edge *e = new Edge(from_node, from_port, to_node, to_port);
    sc->edges->append(e);
    return e;
}

Edge *GraphWidget::findEdge(Node* from_node, int from_port, Node* to_node, int to_port) {
    //Check which cluster that the node is belong to
    SwitchCluster *sc = findSwitchCluster(from_node->id);

    Edge* e = NULL;
    for (int i = 0; i < sc->edges->count(); i++) {
        e = sc->edges->at(i);
        /* return if there is a same edge */
        if ((e->sourceNode() == from_node &&
                e->destNode() == to_node &&
                e->sourcePortNum() == from_port &&
                e->destPortNum() == to_port) ||
                (e->sourceNode() == to_node &&
                e->destNode() == from_node &&
                e->sourcePortNum() == to_port &&
                e->destPortNum() == from_port)) {
            return e;
        }
    }
    e = createEdge(sc, from_node, from_port, to_node, to_port);
    sc->scene->addItem(e);
    /* ogdf */
    edge ogdf_e = G.newEdge(from_node->ogdf_node, to_node->ogdf_node);
    e->ogdf_edge = ogdf_e;
    return e;
}

void GraphWidget::requestTopology() {
    qDebug("requestTopology()");
    QString req_str = "link_status:";
    //rrclient->sendRequest(req_str);
}

void GraphWidget::updateScene() {
    /* prepare the parameters of ogdf */
    int nodeWidth = 30, nodeHeight = 30;
    int siblingDistance = nodeWidth + nodeHeight;

    switch (layout) {
        case PlanarStraightLA:
            try {
                psl.sizeOptimization(true);
                psl.call(GA);
            } catch (int err) {
                qDebug("......PlenerStraightLayout starts with errors!");
            }
            break;
        case PlanarDrawLA:
            try {
                pdl.sizeOptimization(true);
                pdl.call(GA);
            } catch (int err) {
                qDebug("......PlanarDrawLayout starts with errors!");
            }
            break;
        case MixedModelLA:
            try {
                mml.call(GA);
            } catch (int err) {
                qDebug("......MixedModelLayout starts with errors!");
            }
            break;
        case UpwardPlanarizationLA:
            try {
                upl.call(GA);
            } catch (int err) {
                qDebug("......UpwardPlanarizationLayout starts with errors!");
            }
            break;
        default:
            break;
    }

    int width = GA.boundingBox().width() * 1.4, height = GA.boundingBox().height() * 1.4;
    qDebug() << "width,height==>" << width << "," << height;

    curr_cluster->scene->setSceneRect(QRect(-30 - nodeWidth, -30 - nodeHeight, width + nodeWidth, height + nodeHeight));

    GA.setAllWidth(nodeWidth);
    GA.setAllHeight(nodeHeight);

    /* update every position of node using */
    for (int i = 0; i < curr_cluster->nodes->count(); i++) {
        Node *qt_node = curr_cluster->nodes->at(i);
        node n = qt_node->ogdf_node;
        double x = GA.x(n);
        double y = GA.y(n);
        qt_node->setPos(x, y);
        qDebug() << "Node, setPos(x, y)==>" << qt_node->id << "," << x << "," << y;
    }
    this->update();
    // Set the scene of the current cluster
    this->setScene(curr_cluster->scene);
    // clear the graph after it has been displayed
    G.clear();
}

// Obsolete due to not using ZeroMQ

void GraphWidget::updateDataModel(const QList<QByteArray>& reply) {
}

void GraphWidget::updateDataModel(QByteArray& result) {
    qDebug() << "GraphWidget::updateDataModel(QByteArray&): ";

    if (curr_cluster == NULL)
        qDebug() << "curr_cluster is null!";

    QScriptValue sc;
    QScriptEngine engine;
    QString temp = QString(result);
    //sc = engine.evaluate(buffer->buffer());
    sc = engine.evaluate("JSON.parse").call(QScriptValue(), QScriptValueList() << temp);

    if (sc.isArray()) {
        QScriptValueIterator it(sc);
        while (it.hasNext()) {
            it.next();
            QScriptValue switch_link = it.value();
            if (switch_link.isObject()) { // Switch Link Information

                // Find from Node
                QString from_id = switch_link.property("src-switch").toString();
                Node *f_node = findNode(from_id, SwitchKind);
                // Find to Node
                QString to_id = switch_link.property("dst-switch").toString();
                Node *t_node = findNode(to_id, SwitchKind);

                // Find Edge
                QString from_port = switch_link.property("src-port").toString();
                QString to_port = switch_link.property("dst-port").toString();
                QString status = QString("up");
                Edge *edge = findEdge(f_node, from_port.toInt(), t_node, to_port.toInt());
                edge->setStatus(status);
            }
        }
    }
    // update scene
    updateScene();
}

void GraphWidget::updateHostDataModel(QByteArray& result) {

    QScriptValue sc;
    QScriptEngine engine;
    QString temp = QString(result);
    //sc = engine.evaluate(buffer->buffer());
    sc = engine.evaluate("JSON.parse").call(QScriptValue(), QScriptValueList() << temp);

    if (sc.isArray()) {
        QScriptValueIterator it(sc);
        while (it.hasNext()) {
            it.next();
            QScriptValue host = it.value();
            if (host.isObject()) { // Host Information

                // Get host MAC addr
                QScriptValue mac_host = host.property("mac");
                QString mac_str = mac_host.toString();
                //qDebug() << mac_str;
                // Get attached switch
                QScriptValue attpoint = host.property("attachmentPoint");
                if (attpoint.isArray()) {
                    QScriptValueIterator it_attpoint(attpoint);
                    while (it_attpoint.hasNext()) {
                        it_attpoint.next();
                        QScriptValue sw = it_attpoint.value();
                        if (sw.isObject()) {
                            QString sw_str = sw.property("switchDPID").toString();
                            //qDebug() << sw_str;
                            // Find to Node
                            Node *f_node = findNode(sw_str, SwitchKind);
                            SwitchCluster *sc = findSwitchCluster(sw_str);
                            //Find from Node
                            Node *t_node = findHostNode(sc, mac_str, HostKind);
                            // Add Link
                            QString from_port = sw.property("port").toString();
                            QString to_port = "0";
                            QString status = QString("up");
                            Edge *edge = findEdge(f_node, from_port.toInt(), t_node, to_port.toInt());
                            edge->setStatus(status);
                        }
                    }
                }
            }
        }
    }
}

void GraphWidget::abortRequest() {
    qDebug() << "GraphWidget::abortRequest()> Timeout";
}

//! [1]

//! [2]

void GraphWidget::itemMoved() {
    if (!timerId)
        timerId = startTimer(1000 / 25);
}
//! [2]

//! [3]

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Up:
            centerNode->moveBy(0, -20);
            break;
        case Qt::Key_Down:
            centerNode->moveBy(0, 20);
            break;
        case Qt::Key_Left:
            centerNode->moveBy(-20, 0);
            break;
        case Qt::Key_Right:
            centerNode->moveBy(20, 0);
            break;
        case Qt::Key_Plus:
            zoomIn();
            break;
        case Qt::Key_Minus:
            zoomOut();
            break;
        case Qt::Key_Space:
        case Qt::Key_Enter:
            shuffle();
            break;
        default:
            QGraphicsView::keyPressEvent(event);
    }
}
//! [3]

//! [4]

void GraphWidget::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);

    if (force_calculation_flag) {
        QList<Node *> nodes;

        foreach(QGraphicsItem *item, scene()->items()) {
            if (Node * node = qgraphicsitem_cast<Node *>(item))
                nodes << node;
        }

        foreach(Node *node, nodes) {
            node->calculateForces();
        }

        bool itemsMoved = false;

        foreach(Node *node, nodes) {
            if (node->advance())
                itemsMoved = true;
        }

        if (!itemsMoved) {
            killTimer(timerId);
            timerId = 0;
        }
    } else {
        killTimer(timerId);
        timerId = 0;
    }
}
//! [4]

//! [5]

void GraphWidget::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double) 2, -event->delta() / 240.0));
}
//! [5]

//! [6]

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_MAEMO_5)
    // Text
    /*
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    
    
    QString message(tr("Click and drag the nodes around, and zoom with the mouse "
                    "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
     */

#endif
}
//! [6]

//! [7]

void GraphWidget::scaleView(qreal scaleFactor) {
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
//! [7]

void GraphWidget::shuffle() {

    foreach(QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void GraphWidget::zoomIn() {
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut() {
    scaleView(1 / qreal(1.2));
}
