

#ifndef INC_QTSWITCHPLUGIN_H
#define INC_QTSWITCHPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class QtSwitchPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "invariant-corp.Qt.QtSwitchPlugin" )
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    QtSwitchPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool initialized;
};

#endif
