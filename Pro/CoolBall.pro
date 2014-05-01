#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T21:21:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CoolBall
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    helpdialog.cpp \
    gird.cpp \
    ball.cpp \
    utility.cpp \
    highrecord.cpp

HEADERS  += mainwindow.h \
    helpdialog.h \
    gird.h \
    ball.h \
    utility.h \
    highrecord.h

FORMS    += mainwindow.ui \
    helpdialog.ui \
    highrecord.ui
