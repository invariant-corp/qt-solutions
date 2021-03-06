
# Add QT dependencies
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Designer REQUIRED)
FIND_PACKAGE(Qt5UiPlugin REQUIRED)

IF(MSVC AND BUILD_SHARED_LIBS)

ENDIF()

if(WIN32)
    ADD_DEFINITIONS( -DQT_UIPLUGIN_LIB )
    ADD_DEFINITIONS( -DQT_DESIGNER_LIB )
    ADD_DEFINITIONS( -DQT_PLUGIN )
    ADD_DEFINITIONS( -DQDESIGNER_EXPORT_WIDGETS )
endif()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(_SRCS
  qtswitch.cpp
  qtswitchplugin.cpp
  )

set( _HEADERS
   qtswitch.h
   qtswitchplugin.h
)

set(_UI_FORMS
  )

set(_RESOURCES
   qtswitch.qrc
  )
  
QT5_WRAP_UI(_UI_SRCS ${_UI_FORMS})
QT5_ADD_RESOURCES(_QRC_SRCS ${_RESOURCES})

set(TARGET_NAME QtSwitch)

add_library(${TARGET_NAME}
  ${_SRCS}
  ${_UI_SRCS}
  ${_QRC_SRCS}
  ${_HEADERS}
  )

target_link_libraries(${TARGET_NAME} Qt5::Widgets)
target_link_libraries(${TARGET_NAME} Qt5::Designer)
target_link_libraries(${TARGET_NAME} Qt5::UiPlugin)

IF(UNIX)
   set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION} SOVERSION ${MAJOR_VERSION})
endif()

######################### Installation Stuff ###########################
include(GenerateExportHeader)
generate_export_header(${TARGET_NAME})

install(TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}/${TARGET_NAME}
)

install(
  FILES
    ${_HEADERS}
  DESTINATION
    ${INSTALL_INCLUDE_DIR}/${TARGET_NAME}
  COMPONENT
    Devel
)

