
# Add QT dependencies
FIND_PACKAGE(Qt5Widgets REQUIRED)

IF(MSVC AND BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DQT_QTPROPERTYBROWSER_EXPORT)
ENDIF()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(_SRCS
  qtbuttonpropertybrowser.cpp
  qteditorfactory.cpp
  qtgroupboxpropertybrowser.cpp
  qtpropertybrowser.cpp
  qtpropertybrowserutils.cpp
  qtpropertymanager.cpp
  qttreepropertybrowser.cpp
  qtvariantproperty.cpp
  )

set( _HEADERS
	qtpropertybrowser.h
    qtpropertymanager.h
    qteditorfactory.h
    qtvariantproperty.h
    qttreepropertybrowser.h
    qtbuttonpropertybrowser.h
    qtgroupboxpropertybrowser.h
    qtpropertybrowserutils_p.h
	QtAbstractEditorFactoryBase
	QtAbstractPropertyBrowser
	QtAbstractPropertyManager
	QtBoolPropertyManager
	QtBrowserItem
	QtButtonPropertyBrowser
	QtCharEditorFactory
	QtCharPropertyManager
	QtCheckBoxFactory
	QtColorEditorFactory
	QtColorPropertyManager
	QtCursorEditorFactory
	QtCursorPropertyManager
	QtDateEditFactory
	QtDatePropertyManager
	QtDateTimeEditFactory
	QtDateTimePropertyManager
	QtDoublePropertyManager
	QtDoubleSpinBoxFactory
	QtEnumEditorFactory
	QtEnumPropertyManager
	QtFlagPropertyManager
	QtFontEditorFactory
	QtFontPropertyManager
	QtGroupBoxPropertyBrowser
	QtGroupPropertyManager
	QtIntPropertyManager
	QtKeySequenceEditorFactory
	QtKeySequencePropertyManager
	QtLineEditFactory
	QtLocalePropertyManager
	QtPointFPropertyManager
	QtPointPropertyManager
	QtProperty
	QtRectFPropertyManager
	QtRectPropertyManager
	QtScrollBarFactory
	QtSizeFPropertyManager
	QtSizePolicyPropertyManager
	QtSizePropertyManager
	QtSliderFactory
	QtSpinBoxFactory
	QtStringPropertyManager
	QtTimeEditFactory
	QtTimePropertyManager
	QtTreePropertyBrowser
	QtVariantEditorFactory
	QtVariantProperty
	QtVariantPropertyManager
)

set(_UI_FORMS
  )

set(_RESOURCES
  qtpropertybrowser.qrc
  )
  
QT5_WRAP_UI(_UI_SRCS ${_UI_FORMS})
QT5_ADD_RESOURCES(_QRC_SRCS ${_RESOURCES})

set(TARGET_NAME QtPropertyBrowser)

add_library(${TARGET_NAME}
  ${_SRCS}
  ${_UI_SRCS}
  ${_QRC_SRCS}
  ${_HEADERS}
  )

target_link_libraries(${TARGET_NAME} Qt5::Widgets)

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

