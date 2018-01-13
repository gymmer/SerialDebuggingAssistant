#-------------------------------------------------
#
# Project created by QtCreator 2014-10-25T16:26:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialDebuggingAssistant
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserialbase.cpp \
    win_qextserialport.cpp

HEADERS  += mainwindow.h \
    qextserialbase.h \
    win_qextserialport.h

FORMS    += mainwindow.ui

RESOURCES += \
    source.qrc

RC_FILE += SerialDebuggingAssistant.rc
