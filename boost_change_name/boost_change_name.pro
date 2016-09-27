QT += core
QT += concurrent

QT -= gui

TARGET = boost_change_name
CONFIG += console

TEMPLATE = app

SOURCES += main.cpp

include($$PWD/../CppTool.pri)
DESTDIR=$$THIS_PROJECT_DESTDIR


