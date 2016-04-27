#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T10:31:33
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = gateway.ico
VERSION = 1.0.0.1
QMAKE_TARGET_COMPANY = Kontel Co
QMAKE_TARGET_PRODUCT = TCPGateway
QMAKE_TARGET_DESCRIPTION = TCP ports gateway
QMAKE_TARGET_COPYRIGHT = Kontel

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
