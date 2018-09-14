#-------------------------------------------------
#
# Project created by QtCreator 2018-08-01T17:52:38
#
#-------------------------------------------------

QT       += core gui
#QT       += network
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = packetAnalyzer
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
    packet.cpp \
    packetparser.cpp \
    qcustomplot.cpp \
    kalman.cpp \
    kalmanpacketfilter.cpp \
    highpassfilter.cpp \
    complementaryfilter.cpp \
    complementarypacketfilter.cpp \
    form_fft.cpp

HEADERS += \
        mainwindow.h \
    packet.h \
    packetparser.h \
    qcustomplot.h \
    kalman.h \
    kalmanpacketfilter.h \
    highpassfilter.h \
    complementaryfilter.h \
    complementarypacketfilter.h \
    form_fft.h

FORMS += \
        mainwindow.ui \
    form_fft.ui
