QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_2
TEMPLATE = app

CONFIG += c++14

SOURCES += \
    cube.cpp \
    main.cpp \
    mainwindow.cpp \
    mainview.cpp \
    normalshader.cpp \
    phongshader.cpp \
    pyramid.cpp \
    user_input.cpp \
    model.cpp \
    object.cpp

HEADERS += \
    cube.h \
    mainwindow.h \
    mainview.h \
    model.h \
    normalshader.h \
    phongshader.h \
    pyramid.h \
    object.h \
    vertex3d.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
