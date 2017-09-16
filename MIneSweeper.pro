#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T10:10:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MIneSweeper
TEMPLATE = app


SOURCES += main.cpp \
    minesweepermodel.cpp \
    minesweeperview.cpp \
    minesweepercontroller.cpp \
    cell.cpp \
    rightclickedbutton.cpp \
    lcdtimer.cpp \
    lcdcounter.cpp

HEADERS  += \
    minesweepermodel.h \
    minesweeperview.h \
    minesweepercontroller.h \
    cell.h \
    rightclickedbutton.h \
    lcdtimer.h \
    lcdcounter.h

FORMS    +=

RESOURCES += \
    resources.qrc
