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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>

#include "httpget.h"
#include "graphwidget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    
public slots:
    void test();
    void showTopology();
    void doREST(QUrl &url, QString &method, QString &data, int rest_type);
    void layoutChanged(int index);
    void handleTopoTreeSignal(QTreeWidgetItem *item, int column);
    void handleHttpDoneSignal(QByteArray& result, int type);
        
private slots:
    void about();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void createTopoTreeWidget();
    QTreeWidgetItem *addTreeWidget(QTreeWidgetItem *parent, QString &item_name, QString &desc);

    HttpGet *httpget;
    GraphWidget *graphWidget;
    
    QTreeWidget *treeWidget;
            
    QWidget *form;
    QVBoxLayout *verticalLayout;
    
    QGroupBox *groupBox;
    QGroupBox *groupBox2;
    
    QGridLayout *gridLayout;
    QLabel *label_layout;
    QButtonGroup *buttonGroup;
    QRadioButton *pslRadio;
    QRadioButton *pdlRadio;
    QRadioButton *mmlRadio;
    QRadioButton *uplRadio;
    QCheckBox    *forceCalCheck;
    QSpacerItem *verticalSpacer;

    QMenu *topologyMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    
    QToolBar *topologyToolBar;
    QToolBar *editToolBar;
    QToolBar *restToolBar;
    QAction *showtopoAct;
    QAction *doHttpRestRequest;
    QAction *aboutAct;
    QAction *quitAct;
};
//! [0]

#endif