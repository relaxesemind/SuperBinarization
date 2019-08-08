#-------------------------------------------------
#
# Project created by QtCreator 2019-07-17T12:39:32
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14


TARGET = SuperBinarization
TEMPLATE = app


VERSION = 1.0.0.0
QMAKE_TARGET_PRODUCT = SuperBinarization
QMAKE_TARGET_COPYRIGHT = Egor Ivanov Olegovich
QMAKE_TARGET_DESCRIPTION = 3D bin




##################################################
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Controllers/mainwindow.cpp \
    main.cpp \
    Models/inputmodel.cpp \
    Views/imageview.cpp \
    Views/defaultcontrols.cpp \
    Abstract/singletoneclass.cpp \
    Models/appstorage.cpp \
    Models/imagemodel.cpp \
    Models/classmodel.cpp \
    Managers/colorgenerator.cpp \
    Managers/managerslocator.cpp \
    Models/baseareamodel.cpp \
    Models/rectareamodel.cpp \
    Models/polyareamodel.cpp \
    Managers/mathmanager.cpp \
    Models/point3d.cpp

HEADERS += \
    Controllers/mainwindow.h \
    Common/magic.h \
    Common/consts.h \
    Models/inputmodel.h \
    Views/imageview.h \
    Views/defaultcontrols.h \
    Abstract/singletoneclass.h \
    Models/appstorage.h \
    Models/imagemodel.h \
    Models/classmodel.h \
    Views/drawtool.h \
    Managers/colorgenerator.h \
    Managers/managerslocator.h \
    Models/baseareamodel.h \
    Models/rectareamodel.h \
    Models/polyareamodel.h \
    Managers/mathmanager.h \
    Models/point3d.h

FORMS += \
    Forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
