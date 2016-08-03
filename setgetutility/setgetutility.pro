
#CONFIG += console

QT += core gui
QT += widgets

TARGET = setgetutility
TEMPLATE = app


SOURCES += $$PWD/main.cpp
SOURCES += $$PWD/MainWindow.cpp
SOURCES += $$PWD/SetGetUtility.cpp

HEADERS  += $$PWD/MainWindow.hpp
HEADERS  += $$PWD/SetGetUtility.hpp

include($$PWD/../CppTool.pri)
DESTDIR=$$THIS_PROJECT_DESTDIR


