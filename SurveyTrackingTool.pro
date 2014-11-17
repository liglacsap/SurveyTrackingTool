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
    qt/paint2dwidget.cpp \
    qt/conditiondialog.cpp \
    qt/configurationdialog.cpp \
    qt/studyonedialog.cpp \
    qt/studytwodialog.cpp \
    qt/calibrationdialog.cpp \
    qt/maindialog.cpp \
    testdialog.cpp \
    tracking.cpp \
    ems/emstransmission.cpp \
    ems/udpsocket.cpp \
    qt/studytwomasterdialog.cpp



HEADERS  += qt/configurationdialog.h \
    qt/conditiondialog.h \
    qt/studyonedialog.h \
    qt/studytwodialog.h \
    qt/calibrationdialog.h \
    qt/paint2dwidget.h \
    csvfilehandler.h \
    qt/maindialog.h \
    testdialog.h \
    tracking.h \
    csvfilehandler.h \
    ems/emstransmission.h \
    ems/udpsocket.h \
    structures.h \
    qt/trackingdialog.h \
    qt/studytwomasterdialog.h

FORMS    += qt/configurationdialog.ui \
    qt/conditiondialog.ui \
    qt/studyonedialog.ui \
    qt/studytwodialog.ui \
    qt/maindialog.ui \
    qt/calibrationdialog.ui \
    testdialog.ui \
    qt/studytwomasterdialog.ui


win32:LIBS += -L"C:\Users\lab\Desktop\Gil Engel Bachelor Thesis\NatNet_SDK_2.6\lib\x64" -lNatNetLib

RESOURCES += \
    icons.qrc

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Build/Tracking/release/ -lTracking
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Build/Tracking/debug/ -lTracking

INCLUDEPATH += $$PWD/../NatNet_SDK_2.6/include
#INCLUDEPATH += $$PWD/../Tracking
#DEPENDPATH += $$PWD/../Tracking

SOURCES += main.cpp

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui

OTHER_FILES +=

