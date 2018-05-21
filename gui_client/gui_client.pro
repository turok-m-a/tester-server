#-------------------------------------------------
#
# Project created by QtCreator 2018-01-24T15:14:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui_client
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
    testwindow.cpp \
    question.cpp \
    network.cpp \
    moveitem.cpp \
    sequencequestiondrawer.cpp \
    groupquestiondrawer.cpp \
    infowindow.cpp \
    teacherwindow.cpp \
    studcontrolwindow.cpp \
    subjectcontrolwindow.cpp \
    questioncontrol.cpp \
    examcontrol.cpp \
    reportview.cpp \
    adminwindow.cpp \
    loadtest.cpp

HEADERS += \
        mainwindow.h \
    testwindow.h \
    question.h \
    network.h \
    moveitem.h \
    sequencequestiondrawer.h \
    groupquestiondrawer.h \
    constants.h \
    infowindow.h \
    teacherwindow.h \
    studcontrolwindow.h \
    subjectcontrolwindow.h \
    questioncontrol.h \
    examcontrol.h \
    reportview.h \
    adminwindow.h \
    loadtest.h

FORMS += \
        mainwindow.ui \
    testwindow.ui \
    infowindow.ui \
    teacherwindow.ui \
    studcontrolwindow.ui \
    subjectcontrolwindow.ui \
    questioncontrol.ui \
    examcontrol.ui \
    reportview.ui \
    adminwindow.ui
win32: LIBS += -lws2_32
QMAKE_LFLAGS += -static
CONFIG += staticlib console
DISTFILES += \
    classdiagram.qmodel
