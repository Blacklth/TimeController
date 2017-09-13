#-------------------------------------------------
#
# Project created by QtCreator 2017-07-27T16:44:27
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeController
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    curriculum.cpp \
    remindpage.cpp \
    remindlist.cpp \
    datadialog.cpp \
    events.cpp \
    note.cpp

HEADERS  += mainwindow.h \
    curriculum.h \
    remindpage.h \
    remindlist.h \
    datadialog.h \
    events.h \
    note.h

FORMS    += mainwindow.ui \
    curriculum.ui \
    remindpage.ui \
    datadialog.ui \
    events.ui \
    note.ui

RESOURCES += \
    icons.qrc

DISTFILES +=
