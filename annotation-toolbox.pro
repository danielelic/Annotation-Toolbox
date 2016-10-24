QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += c++11
QMAKE_CXXFLAGS += -std=c++0x

TARGET = annotation-toolbox
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp

HEADERS  += mainwindow.h \
    camera.h

FORMS    += mainwindow.ui

DEFINES += APP_NAME=\\\"Annotation_Toolbox\\\"
VERSION = 1.0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

LIBS    += -lopencv_core -lopencv_imgproc -lopencv_highgui \
           -lOpenNI2
