QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_2
TEMPLATE = app

CONFIG += c++14

SOURCES += \
    animation.cpp \
    circleanimation.cpp \
    cube.cpp \
    gouraudshader.cpp \
    jumpanimation.cpp \
    main.cpp \
    mainwindow.cpp \
    mainview.cpp \
    normalshader.cpp \
    phongshader.cpp \
    pyramid.cpp \
    shader.cpp \
    user_input.cpp \
    model.cpp \
    object.cpp \
    utility.cpp

HEADERS += \
    TextureType.h \
    animation.h \
    circleanimation.h \
    cube.h \
    gouraudshader.h \
    jumpanimation.h \
    mainwindow.h \
    mainview.h \
    model.h \
    normalshader.h \
    phongshader.h \
    pyramid.h \
    object.h \
    shader.h \
    vertex3d.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
