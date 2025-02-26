QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_1
TEMPLATE = app

CONFIG += c++14

SOURCES += \
    cube.cpp \
    main.cpp \
    mainwindow.cpp \
    mainview.cpp \
    pyramid.cpp \
    user_input.cpp \
    model.cpp \
    object.cpp

HEADERS += \
    cube.h \
    mainwindow.h \
    mainview.h \
    model.h \
    pyramid.h \
    vertex_3d.h \
    object.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
