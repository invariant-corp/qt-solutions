
#ifndef INC_QTLED_H
#define INC_QTLED_H


#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>



class QDESIGNER_WIDGET_EXPORT QtLed : public QWidget
{
   Q_OBJECT
   Q_PROPERTY( bool value READ value WRITE setValue );
   Q_PROPERTY( QColor color READ color WRITE setColor );
   Q_PROPERTY( QString label READ label WRITE setLabel );
   Q_PROPERTY( bool labelVisible READ labelVisible WRITE setLabelVisible );

   public: 
      QtLed( QWidget* parent=0 );
      QtLed( const QString& label, QWidget* parent=0 );

      bool value() const { return m_value; }
      QColor color() const { return m_color; }
      QString label() const { return m_label; }
      bool labelVisible() const { return m_labelVisible; }

      QSize sizeHint() const;
      QSize minimumSizeHint() const;

   public slots:
      void setValue( bool );
      void setColor( QColor );
      void setLabel( const QString& );
      void setLabelVisible( bool );

      void toggleValue();

   protected:
      void paintEvent( QPaintEvent* );

      bool m_value;
      QColor m_color;
      QString m_label;
      bool m_labelVisible;

      static const int c_textMargin;

}; // end class QtLed

#endif

// EOF
