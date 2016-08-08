QT       += core gui
QT       += widgets

TARGET = memory_test
TEMPLATE = app


SOURCES += \
    Memory.cpp

HEADERS  += \
    Memory.hpp \
    memory_private/Memory.case.hpp \
    memory_private/Memory.pool.hpp \
    memory_private/Memory.template.hpp

INCLUDEPATH += $$PWD/memory_private

include($$PWD/../CppTool.pri)
DESTDIR=$$THIS_PROJECT_DESTDIR


