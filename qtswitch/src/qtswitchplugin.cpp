

#include "qtswitch.h"
#include "qtswitchplugin.h"

#include <QtPlugin>

QtSwitchPlugin::QtSwitchPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QtSwitchPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QtSwitchPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QtSwitchPlugin::createWidget(QWidget *parent)
{
    return new QtSwitch(parent);
}

QString QtSwitchPlugin::name() const
{
    return "QtSwitch";
}

QString QtSwitchPlugin::group() const
{
    return "Lab Widgets";
}

QIcon QtSwitchPlugin::icon() const
{
    return QIcon(":qtswitch.png");
}

QString QtSwitchPlugin::toolTip() const
{
    return "";
}

QString QtSwitchPlugin::whatsThis() const
{
    return "";
}

bool QtSwitchPlugin::isContainer() const
{
    return false;
}

QString QtSwitchPlugin::domXml() const
{
    return "<widget class=\"QtSwitch\" name=\"qSwitch\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>100</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string>Binary Switch</string>\n"
           " </property>\n"
           " <property name=\"value\" >\n"
           " <bool>false</bool>\n"
           " </property>\n"
 		   " <property name=\"type\" >\n"
           " <enum>QSwitch::Vertical</enum>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>Switch widget</string>\n"
           " </property>\n"
           "</widget>\n";
}

QString QtSwitchPlugin::includeFile() const
{
    return "qtswitch.h";
}

