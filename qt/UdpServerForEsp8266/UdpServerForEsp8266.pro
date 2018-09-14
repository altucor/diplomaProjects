#-------------------------------------------------
#
# Project created by QtCreator 2018-06-28T16:08:35
#
#-------------------------------------------------

QT      += core gui
QT      += network
QT      += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DiplomUdpServerForEsp8266
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        udpserverwrapper.cpp \
        qcustomplot.cpp \
    packetparser.cpp \
    kalman.cpp \
    packet.cpp \
    Complementary.cpp

HEADERS += \
        mainwindow.h \
        udpserverwrapper.h \
        qcustomplot.h \
    packetparser.h \
    kalman.h \
    packet.h \
    Complementary.h

FORMS += \
        mainwindow.ui
