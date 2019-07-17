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
    Models/appstorage.cpp

HEADERS += \
    Controllers/mainwindow.h \
    Common/magic.h \
    Common/consts.h \
    Models/inputmodel.h \
    Views/imageview.h \
    Views/defaultcontrols.h \
    Abstract/singletoneclass.h \
    Models/appstorage.h

FORMS += \
    Forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
