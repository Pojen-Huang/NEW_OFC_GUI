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