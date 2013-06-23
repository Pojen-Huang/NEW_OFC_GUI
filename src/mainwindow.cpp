//! [0]
#include <QtGui>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>

#include "mainwindow.h"
//! [0]

//! [1]

MainWindow::MainWindow() {
    /* ZeroMQ connection protocol setting */
    QString reqrep_conn = "tcp://127.0.0.1:4445";
    QString pubsub_conn = "tcp://127.0.0.1:4444";
    QString rest_conn = "http://localhost:8080";

    graphWidget = new GraphWidget(this);
    /* build connection to OpenFlow Controller via Requester/Responder and Publisher/Subscriber */
    //graphWidget->setZeroMQConn(reqrep_conn, pubsub_conn);
    setCentralWidget(graphWidget);
    //qRegisterMetaType<HttpGet::HTTPRequestType>("HTTPRequestType");

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();
    createTopoTreeWidget();
    setWindowTitle(tr("OpenFlow Controller GUI"));
    setUnifiedTitleAndToolBarOnMac(true);

    /* HTTP REST Client */
    httpget = new HttpGet(this);
    connect(httpget, SIGNAL(done(QByteArray&, int)),
            this, SLOT(handleHttpDoneSignal(QByteArray&, int)));

    /* Get Switch Clusters information  */
    QUrl url = QUrl(rest_conn + "/wm/topology/switchclusters/json");
    QString method = "GET";
    QString data = "";
    this->doREST(url, method, data, REST_TOPO_SWITCHCLUSTERS);
}
//! [1]

void MainWindow::layoutChanged(int index) {

    switch (index) {
        case 0:
            graphWidget->setLayout(PlanarStraightLA);
            break;
        case 1:
            graphWidget->setLayout(PlanarDrawLA);
            break;
        case 2:
            graphWidget->setLayout(MixedModelLA);
            break;
        case 3:
            graphWidget->setLayout(UpwardPlanarizationLA);
            break;
        default:
            break;
    }
}

void MainWindow::test() {
    qDebug() << "This is for testing...";
}

void MainWindow::showTopology() {

    if (forceCalCheck->isChecked()) {
        graphWidget->setForceCalculation(true);
    } else {
        graphWidget->setForceCalculation(false);
    }
    graphWidget->requestTopology();
}

void MainWindow::doREST(QUrl &url, QString &method, QString &data, int rest_type) {
    httpget->doHttpRequest(url, method, data, rest_type);
}

void MainWindow::handleTopoTreeSignal(QTreeWidgetItem *item, int column) {
    qDebug() << item->text(column) << endl;
    if( item->text(1) == "cluster" ){
        // Change cluster
        QString curr_cluster = item->text(0);
        graphWidget->setCurrentCluster(curr_cluster);
        QUrl url = QUrl("http://localhost:8080/wm/device/");
        QString method = "GET";
        QString data = "";
        this->doREST(url, method, data, REST_DEVICES);
    }
    
    
}

void MainWindow::handleHttpDoneSignal(QByteArray& result, int type) {

    QScriptValue sc;
    QScriptEngine engine;
    QString temp = QString(result);
    //sc = engine.evaluate(buffer->buffer());
    sc = engine.evaluate("JSON.parse").call(QScriptValue(), QScriptValueList() << temp);

    // Because of getting new cluster information,
    // we need to reset all the clusters in graphWidget.

    if (type == REST_TOPO_SWITCHCLUSTERS) {
        QString c_str = QString("cluster");
        QString sw_str = QString("switch");
        graphWidget->deleteSwitchCluster();
        //PhysicalNetwork TreeItem
        QList<QTreeWidgetItem *> phy_nt_item_list = treeWidget->findItems("PhysicalNetwork",
                Qt::MatchExactly | Qt::MatchRecursive, 0);
        QTreeWidgetItem *phy_nt_item = phy_nt_item_list.at(0);
        QTreeWidgetItem *cluster_item;
        QTreeWidgetItem *item;

        QScriptValueIterator it(sc);
        while (it.hasNext()) {
            it.next();
            QScriptValue cluster = it.value();
            //Add new cluster tree item
            QString cluster_name = it.name();
            cluster_item = addTreeWidget(phy_nt_item, cluster_name, c_str);

            if (cluster.isObject()) { // Cluster Information
                QScriptValueIterator cluster_it(cluster);
                while (cluster_it.hasNext()) {
                    cluster_it.next();
                    QScriptValue sw = cluster_it.value();
                    if (sw.isString()) {
                        //Add new switch tree item
                        QString sw_name = sw.toString();
                        item = addTreeWidget(cluster_item, sw_name, sw_str);

                        //Add Switch Cluster and Switches
                        graphWidget->addSwitchCluster(cluster_name, sw_name);
                    }
                }
            }
        }
    } else if (type == REST_TOPO_LINKS) {
        // 4. Parse and update topology link data
        graphWidget->updateDataModel(result);
    } else if (type == REST_DEVICES) {
        // 1. Reset graph objects
        graphWidget->resetSwitchCluster();
        // 2. Parse and update host data
        graphWidget->updateHostDataModel(result);
        // 3. Request topology link data
        QUrl url = QUrl("http://localhost:8080/wm/topology/links/json");
        QString method = "GET";
        QString data = "";
        this->doREST(url, method, data, REST_TOPO_LINKS);

    } else {

    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("OpenFlow Controller GUI"),
            tr("The <b>OpenFlow Controller GUI</b> demonstrates how to "
            "use Qt's components to build a interactive GUI."));
}

void MainWindow::createActions() {
    showtopoAct = new QAction(QIcon(":/images/topology.png"), tr("&Show Topology"), this);
    showtopoAct->setStatusTip(tr("Show Topology"));
    connect(showtopoAct, SIGNAL(triggered()), this, SLOT(showTopology()));

    doHttpRestRequest = new QAction(QIcon(":/images/REST.png"), tr("&Do REST"), this);
    doHttpRestRequest->setStatusTip(tr("Do REST Http Request"));
    connect(doHttpRestRequest, SIGNAL(triggered()), this, SLOT(test()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus() {
    viewMenu = menuBar()->addMenu(tr("&View"));
    menuBar()->addSeparator();

    topologyMenu = menuBar()->addMenu(tr("&Topology"));
    topologyMenu->addAction(showtopoAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars() {
    topologyToolBar = addToolBar(tr("Topology"));
    topologyToolBar->addAction(showtopoAct);
    restToolBar = addToolBar(tr("REST"));
    restToolBar->addAction(doHttpRestRequest);

    editToolBar = addToolBar(tr("Edit"));
}

//! [8]

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}
//! [8]

//! [9]

void MainWindow::createDockWindows() {
    groupBox = new QGroupBox(this);
    groupBox->setTitle(QString::fromUtf8("Layout"));
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setMaximumSize(QSize(16777215, 16777215));

    pslRadio = new QRadioButton(groupBox);
    pslRadio->setText(QString::fromUtf8("PlenerStraightLayout"));
    pslRadio->setObjectName(QString::fromUtf8("pslRadio"));
    pslRadio->setMaximumSize(QSize(16777215, 40));
    pslRadio->setLayoutDirection(Qt::LeftToRight);
    pslRadio->setChecked(true);

    pdlRadio = new QRadioButton(groupBox);
    pdlRadio->setText(QString::fromUtf8("PlanarDrawLayout"));
    pdlRadio->setObjectName(QString::fromUtf8("pdlRadio"));
    pdlRadio->setMaximumSize(QSize(16777215, 40));
    pdlRadio->setLayoutDirection(Qt::LeftToRight);

    mmlRadio = new QRadioButton(groupBox);
    mmlRadio->setText(QString::fromUtf8("MixedModelLayout"));
    mmlRadio->setObjectName(QString::fromUtf8("mmlRadio"));
    mmlRadio->setMaximumSize(QSize(16777215, 40));
    mmlRadio->setLayoutDirection(Qt::LeftToRight);

    uplRadio = new QRadioButton(groupBox);
    uplRadio->setText(QString::fromUtf8("UpwardPlanarizationLayout"));
    uplRadio->setObjectName(QString::fromUtf8("uplRadio"));
    uplRadio->setMaximumSize(QSize(16777215, 40));

    forceCalCheck = new QCheckBox(groupBox);
    forceCalCheck->setText(QString::fromUtf8("ForceCalculation"));

    groupBox2 = new QGroupBox(this);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(pslRadio);
    buttonGroup->addButton(pdlRadio);
    buttonGroup->addButton(mmlRadio);
    buttonGroup->addButton(uplRadio);
    buttonGroup->setId(pslRadio, PlanarStraightLA);
    buttonGroup->setId(pdlRadio, PlanarDrawLA);
    buttonGroup->setId(mmlRadio, MixedModelLA);
    buttonGroup->setId(uplRadio, UpwardPlanarizationLA);

    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(layoutChanged(int)));

    gridLayout = new QGridLayout(groupBox);
    gridLayout->addWidget(pslRadio, 0, 0, 1, 1);
    gridLayout->addWidget(pdlRadio, 1, 0, 1, 1);
    gridLayout->addWidget(mmlRadio, 2, 0, 1, 1);
    gridLayout->addWidget(uplRadio, 3, 0, 1, 1);
    gridLayout->addWidget(forceCalCheck, 7, 0, 1, 1);

    //label_layout = new QLabel(groupBox);
    //label_layout->setObjectName(QString::fromUtf8("label_layout"));

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->addWidget(groupBox);
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer);

    form = new QWidget(this);
    form->setLayout(verticalLayout);

    QDockWidget *dock = new QDockWidget(tr("Settings"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(form);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

}
//! [9]

//! [10]

void MainWindow::createTopoTreeWidget() {
    treeWidget = new QTreeWidget(this);
    treeWidget->setWindowTitle("Network Topology");
    treeWidget->resize(300, 400);

    //Define column name
    QStringList columnTitle;
    columnTitle.append("Network");
    columnTitle.append("Description");
    treeWidget->setHeaderLabels(columnTitle);

    QTreeWidgetItem *tree_root = new QTreeWidgetItem(treeWidget);
    tree_root->setText(0, "Topology");
    treeWidget->addTopLevelItem(tree_root);

    QTreeWidgetItem *tree_phy_topo = new QTreeWidgetItem(tree_root);
    tree_phy_topo->setText(0, "PhysicalNetwork");
    tree_root->addChild(tree_phy_topo);

    QTreeWidgetItem *tree_virt_topo = new QTreeWidgetItem(tree_root);
    tree_virt_topo->setText(0, "VirtualTenantNetwork");
    tree_root->addChild(tree_virt_topo);

    QDockWidget *dock = new QDockWidget(tr("Topology"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(treeWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
            this, SLOT(handleTopoTreeSignal(QTreeWidgetItem *, int)));
}
//! [10]

QTreeWidgetItem *MainWindow::addTreeWidget(QTreeWidgetItem *parent, QString &item_name, QString &desc) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, item_name);
    item->setText(1, desc);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    parent->addChild(item);
    return item;
}

