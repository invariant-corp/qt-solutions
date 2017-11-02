TEMPLATE = lib
QT += designer uiplugin
CONFIG -= debug release debug_and_release
CONFIG *= plugin debug_and_release build_all
TARGET = $$qtLibraryTarget( $$TARGET )
#QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/designer

HEADERS = \
   qtswitch.h \
   qtswitchplugin.h
   
SOURCES = \
   qtswitch.cpp \
   qtswitchplugin.cpp
   
RESOURCES = qtswitch.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$HEADERS
sources.path = $$[QT_INSTALL_PREFIX]/src/3rdparty/qt_addons/qtswitch
INSTALLS += sources target

build_pass:CONFIG( Debug ) {
   OBJECTS_DIR = debug
   TARGET = $$join(TARGET,,,d)
}
build_pass:CONFIG( Release ) {
   OBJECTS_DIR = release
}

build_pass {
   win32 {
      mylib.files = $$OBJECTS_DIR/*.lib
      mylib.path = $$[QT_INSTALL_LIBS]
      mylib.extras = copy $$OBJECTS_DIR/*.lib $$[QT_INSTALL_LIBS]
      INSTALLS += mylib
      mydll.files = $$OBJECTS_DIR/*.dll
      mydll.path = $$[QT_INSTALL_BINS]
      mydll.extras = copy $$OBJECTS_DIR/*.dll $$[QT_INSTALL_BINS]
      INSTALLS += mydll
   }
}