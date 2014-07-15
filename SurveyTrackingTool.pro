#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T10:44:24
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SurveyTrackingTool
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    configurationdialog.cpp \
    paint2dwidget.cpp \
    emstransmission.cpp \
    udpsocket.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    globals.h \
    configurationdialog.h \
    paint2dwidget.h \
    emstransmission.h \
    udpsocket.h

FORMS    += mainwindow.ui \
    configurationdialog.ui

INCLUDEPATH += "C:\Users\lab\Desktop\Gil Engel Bachelor Thesis\NatNet_SDK_2.6\include"

win32:LIBS += -L"C:\Users\lab\Desktop\Gil Engel Bachelor Thesis\NatNet_SDK_2.6\lib\x64" -lNatNetLib

LIBS += -lGLU

RESOURCES += \
    icons.qrc


QMAKE_CXXFLAGS += -std=c++0x
