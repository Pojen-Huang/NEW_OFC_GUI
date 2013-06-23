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
