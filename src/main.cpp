
#include <QtGui>
#include "graphwidget.h"
#include "mainwindow.h"

int main(int argc, char **argv)
{
    
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(NEW_OFC_GUI);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //Q_INIT_RESOURCE(OFC_GUI);
    MainWindow *mw = new MainWindow();

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
    mainWindow.menuBar()->addAction("Shuffle", widget, SLOT(shuffle()));
    mainWindow.menuBar()->addAction("Zoom In", widget, SLOT(zoomIn()));
    mainWindow.menuBar()->addAction("Zoom Out", widget, SLOT(zoomOut()));
    mainWindow.showMaximized();
#else
    mw->resize(1000, 600);
    mw->show();
#endif
    return app.exec();
}
