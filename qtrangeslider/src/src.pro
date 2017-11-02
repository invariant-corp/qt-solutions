TEMPLATE = lib
greaterThan(QT_MAJOR_VERSION, 4): QT *= widgets
CONFIG -= debug release debug_and_release
CONFIG *= debug_and_release build_all
lessThan(QT_MAJOR_VERSION, 5): CONFIG *= designer
greaterThan(QT_MAJOR_VERSION, 4): QT *= designer
QT += gui widgets network
TARGET = qxtspanslider

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0

SOURCES += \
        qtrangeslider.cpp
HEADERS += \
    qtrangeslider.h
    
win32:DEFINES += QT_QTPROPERTYBROWSER_EXPORT
