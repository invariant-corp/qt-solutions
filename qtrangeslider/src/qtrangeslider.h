#ifndef INCLUDE_QTRANGESLIDER_H
#define INCLUDE_QTRANGESLIDER_H


#include <QSlider>
#include <QStyle>


#if defined(Q_OS_WIN)
#  if !defined(QT_QTRANGESLIDER_EXPORT) && !defined(QT_QTRANGESLIDER_IMPORT)
#    define QT_QTRANGESLIDER_EXPORT
#  elif defined(QT_QTRANGESLIDER_IMPORT)
#    if defined(QT_QTRANGESLIDER_EXPORT)
#      undef QT_QTRANGESLIDER_EXPORT
#    endif
#    define QT_QTRANGESLIDER_EXPORT __declspec(dllimport)
#  elif defined(QT_QTRANGESLIDER_EXPORT)
#    undef QT_QTRANGESLIDER_EXPORT
#    define QT_QTRANGESLIDER_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTRANGESLIDER_EXPORT
#endif


class QStylePainter;
class QStyleOptionSlider;


class QT_QTRANGESLIDER_EXPORT QtRangeSlider : public QSlider
{
   Q_OBJECT
   Q_PROPERTY(int lowerValue READ lowerValue WRITE setLowerValue)
   Q_PROPERTY(int upperValue READ upperValue WRITE setUpperValue)
   Q_PROPERTY(int lowerPosition READ lowerPosition WRITE setLowerPosition)
   Q_PROPERTY(int upperPosition READ upperPosition WRITE setUpperPosition)
   Q_PROPERTY(HandleMovementMode handleMovementMode READ handleMovementMode WRITE setHandleMovementMode)
   Q_ENUMS(HandleMovementMode)

   public:
      explicit QtRangeSlider(QWidget* parent = 0);
      explicit QtRangeSlider(Qt::Orientation orientation, QWidget* parent = 0);
      virtual ~QtRangeSlider();

      enum HandleMovementMode
      {
         FreeMovement,
         NoCrossing,
         NoOverlapping
      };

      enum SpanHandle
      {
         NoHandle,
         LowerHandle,
         UpperHandle
      };

      HandleMovementMode handleMovementMode() const;
      void setHandleMovementMode(HandleMovementMode mode);

      int lowerValue() const;
      int upperValue() const;

      int lowerPosition() const;
      int upperPosition() const;

   public Q_SLOTS:
      void setLowerValue(int lower);
      void setUpperValue(int upper);
      void setSpan(int lower, int upper);

      void setLowerPosition(int lower);
      void setUpperPosition(int upper);

   Q_SIGNALS:
      void spanChanged(int lower, int upper);
      void lowerValueChanged(int lower);
      void upperValueChanged(int upper);

      void lowerPositionChanged(int lower);
      void upperPositionChanged(int upper);

      void sliderPressed(SpanHandle handle);

   protected:
      virtual void keyPressEvent(QKeyEvent* event);
      virtual void mousePressEvent(QMouseEvent* event);
      virtual void mouseMoveEvent(QMouseEvent* event);
      virtual void mouseReleaseEvent(QMouseEvent* event);
      virtual void paintEvent(QPaintEvent* event);

   public:
      void initStyleOption( QStyleOptionSlider* option, QtRangeSlider::SpanHandle handle = QtRangeSlider::UpperHandle ) const;
      int pick( const QPoint& pt ) const
      {
         return this->orientation() == Qt::Horizontal ? pt.x() : pt.y();
      }
      int pixelPosToRangeValue( int pos ) const;
      void handleMousePress( const QPoint& pos, QStyle::SubControl& control, int value, QtRangeSlider::SpanHandle handle );
      void drawHandle( QStylePainter* painter, QtRangeSlider::SpanHandle handle ) const;
      void setupPainter( QPainter* painter, Qt::Orientation orientation, qreal x1, qreal y1, qreal x2, qreal y2 ) const;
      void drawSpan( QStylePainter* painter, const QRect& rect ) const;
      void triggerAction( QAbstractSlider::SliderAction action, bool main );
      void swapControls();

      int lower;
      int upper;
      int lowerPos;
      int upperPos;
      int offset;
      int position;
      QtRangeSlider::SpanHandle lastPressed;
      QtRangeSlider::SpanHandle mainControl;
      QStyle::SubControl lowerPressed;
      QStyle::SubControl upperPressed;
      QtRangeSlider::HandleMovementMode movement;
      bool firstMovement;
      bool blockTracking;

      public Q_SLOTS:
      void updateRange( int min, int max );
      void movePressedHandle();

}; // end class QtRangeSlider


#endif // INCLUDE_QTRANGESLIDER_H
