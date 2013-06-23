######################################################################
# Automatically generated by qmake (2.01a) Tue May 15 17:41:12 2012
# Modifyed by Danny Liu for integrating with ZeroMQ
######################################################################

TEMPLATE = app
TARGET = 

INCLUDEPATH += . headers \
               /home/liudanny/SourceCode/zeromq2-1/include \
	       /home/liudanny/SourceCode/qt4_projects/OGDF \
	       /home/liudanny/SourceCode/jansson-2.3.1/src


# Input
HEADERS += headers/annotation.h \
           headers/edge.h \
           headers/graphwidget.h \
           headers/node.h \
           headers/nzmqt/nzmqt.hpp \
           headers/nzmqt/common/Tools.h \
           headers/nzmqt/pubsub/PubSubClient.h \
           headers/nzmqt/pubsub/PubSubServer.h \
           headers/nzmqt/reqrep/ReqRepClient.h \
           headers/nzmqt/reqrep/ReqRepServer.h \
           headers/mainwindow.h \
           headers/httpget.h


SOURCES += src/annotation.cpp \
           src/edge.cpp \
           src/graphwidget.cpp \
           src/main.cpp \
           src/node.cpp \
           src/mainwindow.cpp \
           src/httpget.cpp

RESOURCES += NEW_OFC_GUI.qrc

LIBS += -lzmq \
        -lpthread \
        -L/home/liudanny/SourceCode/qt4_projects/OGDF/_release \
        -lOGDF \
	-ljansson

QT += network \
      script


FORMS += \
    mainwindow.ui
