

#ifndef INC_QTSWITCH_H
#define INC_QTSWITCH_H

#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT QtSwitch : public QWidget
{
   Q_OBJECT
   Q_ENUMS( SwitchType )
   Q_PROPERTY( bool value READ value WRITE setValue );
   Q_PROPERTY( SwitchType type READ type WRITE setType );

   public: 
      enum SwitchType { Horizontal, Vertical };

      QtSwitch( QWidget* parent=0 );

      bool value() const { return m_value; }

      SwitchType type() const { return m_type; }

   public slots:
      void setValue( bool );
      void setType( SwitchType );
      void toggleValue();

   signals:
      void valueChanged( bool );

   protected:
      bool m_value;
      SwitchType m_type;
      void paintEvent( QPaintEvent* );
      void mouseReleaseEvent( QMouseEvent* );
      void keyReleaseEvent( QKeyEvent* );

}; // end class QtSwitch

#endif

// EOF
