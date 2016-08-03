
QT       += core gui
QT       += widgets

TARGET = empertyTest
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.hpp

include($$PWD/../CppTool.pri)
DESTDIR=$$THIS_PROJECT_DESTDIR


