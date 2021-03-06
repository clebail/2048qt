#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T12:57:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2048qt
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

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
        CMainWindow.cpp \
    CWGame.cpp \
    CDeplacement.cpp \
    CResolutionDialog.cpp \
    CGenetic.cpp \
    CGamer.cpp \
    CNeurone.cpp \
    CPerceptron.cpp

HEADERS += \
    CMainWindow.h \
    CWGame.h \
    CDeplacement.h \
    common.h \
    CResolutionDialog.h \
    CGenetic.h \
    CGamer.h \
    CNeurone.h \
    CPerceptron.h

FORMS += \
        CMainWindow.ui \
    CResolutionDialog.ui

RESOURCES += \
    2048.qrc
