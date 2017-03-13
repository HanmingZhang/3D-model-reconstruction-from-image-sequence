#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T23:36:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    cplyloader.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    cplyloader.h

FORMS    += mainwindow.ui

LIBS     += -lglut
LIBS     += -lGLU
