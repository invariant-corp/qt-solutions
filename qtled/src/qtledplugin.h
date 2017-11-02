

#ifndef INCLUDE_QTLEDPLUGIN_H
#define INCLUDE_QTLEDPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class QtLedPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "invariant-corp.Qt.QtLedPlugin" )
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    QtLedPlugin(QObject *parent = 0);

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
