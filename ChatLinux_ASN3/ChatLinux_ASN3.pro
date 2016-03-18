#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T13:33:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatLinux_ASN3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connectdialog.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    connectdialog.h \
    ../network.h \
    network.h \
    client.h

FORMS    += mainwindow.ui \
    connectdialog.ui

LIBS += -pthread
