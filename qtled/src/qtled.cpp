
#include "qtled.h"
#include <QColor>
#include <QtGlobal>
#include <QtGui>
#include <QPolygon>
#include <QFont>
#include <QFontMetrics>
#include <QStyle>

#include <iostream>


const int QtLed::c_textMargin = 3;


QtLed::QtLed( QWidget* parent )
: QWidget( parent ),
  m_value( false ),
  m_color( Qt::red ),
  m_labelVisible( false )
{
   this->setMinimumSize( QSize( 16, 16 ) );
   
} // end constructor


QtLed::QtLed( const QString& label, QWidget* parent )
: QWidget( parent ),
  m_value( false ),
  m_color( Qt::red ),
  m_label( label ),
  m_labelVisible( true )
{
   this->setMinimumSize( QSize( 16, 16 ) );
   
} // end constructor


QSize QtLed::sizeHint() const
{
   QSize size = this->minimumSize();

   if ( m_labelVisible == true )
   {
      QFontMetrics metrics = this->fontMetrics();
      QSize fontSize = this->style()->itemTextRect( metrics, QRect(), Qt::TextShowMnemonic, false, m_label ).size();
      size.setWidth( size.width() + fontSize.width() + c_textMargin + c_textMargin );
   }

   return size;

} // end sizeHint


QSize QtLed::minimumSizeHint() const
{
   return this->sizeHint();

} // end minimumSizeHint


void QtLed::paintEvent( QPaintEvent* )
{
   QPainter painter( this );
   painter.save();

   painter.setRenderHints( QPainter::Antialiasing, true );

   painter.setWindow( this->rect() );

   painter.setPen( Qt::white );
   painter.setPen( Qt::darkGray );

   int height = this->rect().height();
   QRect rect = this->rect();
   rect.setWidth( height );
   rect.setTop( rect.top() + 1 );
   rect.setBottom( rect.bottom() - 1 );
   rect.setLeft( rect.left() + 1 );
   rect.setRight( rect.right() - 1 );

   QRadialGradient radialGrad( rect.topLeft() + QPointF( rect.width() / 3.5, rect.height() / 3.5 ), rect.width() / 4.0 );
   radialGrad.setColorAt( 0, Qt::white );

   if ( m_value )
      radialGrad.setColorAt( 1, m_color);
   else
      radialGrad.setColorAt( 1, Qt::lightGray );

   QBrush brush( radialGrad );
   painter.setBrush( brush );
   painter.setPen( QPen( Qt::black, 0.5 ) );
   painter.drawEllipse( rect );

   painter.restore();

   if ( m_labelVisible == true )
   {
      QRect textRect = this->rect();
      textRect.setLeft( textRect.left() + height + c_textMargin );
      painter.setPen( Qt::black );
      painter.drawText( textRect, Qt::AlignVCenter, m_label );
   }

} // end paintEvent


void QtLed::setColor( QColor newColor )
{
   m_color = newColor;
   this->update();

} // end setColor


void QtLed::setValue( bool value )
{
   m_value=value;
   this->update();

} // end setValue


void QtLed::setLabelVisible( bool value )
{
   m_labelVisible=value;
   this->update();
   this->updateGeometry();

} // end setLabelVisible


void QtLed::setLabel( const QString& label )
{
   m_label = label;
   this->update();
   this->updateGeometry();

} // end setLabel


void QtLed::toggleValue()
{ 
	m_value = !m_value;
	this->update();

} // end toggleValue

// EOF
