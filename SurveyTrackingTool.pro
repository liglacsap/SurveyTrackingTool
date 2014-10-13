#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T10:44:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SurveyTrackingTool
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    configurationdialog.cpp \
    paint2dwidget.cpp \
    emstransmission.cpp \
    udpsocket.cpp \
    conditiondialog.cpp \
    tracking.cpp

HEADERS  += mainwindow.h \
    globals.h \
    configurationdialog.h \
    paint2dwidget.h \
    emstransmission.h \
    udpsocket.h \
    csvfilehandler.h \
    conditiondialog.h \
    tracking.h \
    math.h

FORMS    += mainwindow.ui \
    configurationdialog.ui \
    conditiondialog.ui

INCLUDEPATH += "C:\Users\lab\Desktop\Gil Engel Bachelor Thesis\NatNet_SDK_2.6\include"

win32:LIBS += -L"C:\Users\lab\Desktop\Gil Engel Bachelor Thesis\NatNet_SDK_2.6\lib\x64" -lNatNetLib


RESOURCES += \
    icons.qrc


