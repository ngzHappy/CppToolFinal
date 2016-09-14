QT       += core gui
QT       += widgets

QMAKE_CXXFLAGS += -ftemplate-depth=4100

INCLUDEPATH+=$$PWD/../memoryTest/memory_private

TARGET = memory_test_new
TEMPLATE = app


SOURCES += \
    main.cpp \
    Memory.cpp

HEADERS  += \
    Memory.hpp \
    Allocator.hpp \
    MemoryStatic.hpp \
    MakeShared.hpp \
    MakeFunction.hpp

INCLUDEPATH += $$PWD/memory_private

include($$PWD/../CppTool.pri)
DESTDIR=$$THIS_PROJECT_DESTDIR

