#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T10:31:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerGate
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    deborahserver.cpp \
    deborahsocket.cpp \
    deborahgatesocket.cpp \
    deborahmastersocket.cpp \
    deborahlocalsocket.cpp

HEADERS  += mainwindow.h \
    deborahserver.h \
    deborahsocket.h \
    deborahgatesocket.h \
    deborahmastersocket.h \
    deborahlocalsocket.h

FORMS    += mainwindow.ui
