

#include "qtled.h"
#include "qtledplugin.h"

#include <QtPlugin>

QtLedPlugin::QtLedPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QtLedPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QtLedPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QtLedPlugin::createWidget(QWidget *parent)
{
    return new QtLed(parent);
}

QString QtLedPlugin::name() const
{
    return "QtLed";
}

QString QtLedPlugin::group() const
{
    return "Lab Widgets";
}

QIcon QtLedPlugin::icon() const
{
    return QIcon(":qtled.png");
}

QString QtLedPlugin::toolTip() const
{
    return "";
}

QString QtLedPlugin::whatsThis() const
{
    return "";
}

bool QtLedPlugin::isContainer() const
{
    return false;
}

QString QtLedPlugin::domXml() const
{
    return "<widget class=\"QtLed\" name=\"qLed\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>100</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string>Binary Led</string>\n"
           " </property>\n"
           " <property name=\"value\" >\n"
           " <bool>false</bool>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>Led widget</string>\n"
           " </property>\n"
 		   " <property name=\"color\" >\n"
           " <enum>QLed::Red</enum>\n"
           " </property>\n"                     
           "</widget>\n";
}

QString QtLedPlugin::includeFile() const
{
    return "qtled.h";
}

