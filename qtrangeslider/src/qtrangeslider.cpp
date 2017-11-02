
#include "qtrangeslider.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QStylePainter>
#include <QStyleOptionSlider>


void QtRangeSlider::initStyleOption(QStyleOptionSlider* option, QtRangeSlider::SpanHandle handle) const
{
    QSlider::initStyleOption( option );
    option->sliderPosition = (handle == QtRangeSlider::LowerHandle ? lowerPos : upperPos);
    option->sliderValue = (handle == QtRangeSlider::LowerHandle ? lower : upper);
}

int QtRangeSlider::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    int sliderMin = 0;
    int sliderMax = 0;
    int sliderLength = 0;
    const QSlider* p = this;
    const QRect gr = p->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, p);
    const QRect sr = p->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, p);
    if (p->orientation() == Qt::Horizontal)
    {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    }
    else
    {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(p->minimum(), p->maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

void QtRangeSlider::handleMousePress(const QPoint& pos, QStyle::SubControl& control, int value, QtRangeSlider::SpanHandle handle)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt, handle);
    QtRangeSlider* p = this;
    const QStyle::SubControl oldControl = control;
    control = p->style()->hitTestComplexControl(QStyle::CC_Slider, &opt, pos, p);
    const QRect sr = p->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, p);
    if (control == QStyle::SC_SliderHandle)
    {
        position = value;
        offset = pick(pos - sr.topLeft());
        lastPressed = handle;
        p->setSliderDown(true);
        emit p->sliderPressed(handle);
    }
    if (control != oldControl)
        p->update(sr);
}

void QtRangeSlider::setupPainter(QPainter* painter, Qt::Orientation orientation, qreal x1, qreal y1, qreal x2, qreal y2) const
{
    QColor highlight = this->palette().color(QPalette::Highlight);
    QLinearGradient gradient(x1, y1, x2, y2);
    gradient.setColorAt(0, highlight.dark(120));
    gradient.setColorAt(1, highlight.light(108));
    painter->setBrush(gradient);

    if (orientation == Qt::Horizontal)
        painter->setPen(QPen(highlight.dark(130), 0));
    else
        painter->setPen(QPen(highlight.dark(150), 0));
}

void QtRangeSlider::drawSpan(QStylePainter* painter, const QRect& rect) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    const QSlider* p = this;

    // area
    QRect groove = p->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, p);
    if (opt.orientation == Qt::Horizontal)
        groove.adjust(0, 0, -1, 0);
    else
        groove.adjust(0, 0, 0, -1);

    // pen & brush
    painter->setPen(QPen(p->palette().color(QPalette::Dark).light(110), 0));
    if (opt.orientation == Qt::Horizontal)
        setupPainter(painter, opt.orientation, groove.center().x(), groove.top(), groove.center().x(), groove.bottom());
    else
        setupPainter(painter, opt.orientation, groove.left(), groove.center().y(), groove.right(), groove.center().y());

    // draw groove
    painter->drawRect(rect.intersected(groove));
}

void QtRangeSlider::drawHandle(QStylePainter* painter, QtRangeSlider::SpanHandle handle) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt, handle);
    opt.subControls = QStyle::SC_SliderHandle;
    QStyle::SubControl pressed = (handle == QtRangeSlider::LowerHandle ? lowerPressed : upperPressed);
    if (pressed == QStyle::SC_SliderHandle)
    {
        opt.activeSubControls = pressed;
        opt.state |= QStyle::State_Sunken;
    }
    painter->drawComplexControl(QStyle::CC_Slider, opt);
}

void QtRangeSlider::triggerAction(QAbstractSlider::SliderAction action, bool main)
{
    int value = 0;
    bool no = false;
    bool up = false;
    const int min = this->minimum();
    const int max = this->maximum();
    const QtRangeSlider::SpanHandle altControl = (mainControl == QtRangeSlider::LowerHandle ? QtRangeSlider::UpperHandle : QtRangeSlider::LowerHandle);

    blockTracking = true;

    switch (action)
    {
    case QAbstractSlider::SliderSingleStepAdd:
        if ((main && mainControl == QtRangeSlider::UpperHandle) || (!main && altControl == QtRangeSlider::UpperHandle))
        {
            value = qBound(min, upper + this->singleStep(), max);
            up = true;
            break;
        }
        value = qBound(min, lower + this->singleStep(), max);
        break;
    case QAbstractSlider::SliderSingleStepSub:
        if ((main && mainControl == QtRangeSlider::UpperHandle) || (!main && altControl == QtRangeSlider::UpperHandle))
        {
            value = qBound(min, upper - this->singleStep(), max);
            up = true;
            break;
        }
        value = qBound(min, lower - this->singleStep(), max);
        break;
    case QAbstractSlider::SliderToMinimum:
        value = min;
        if ((main && mainControl == QtRangeSlider::UpperHandle) || (!main && altControl == QtRangeSlider::UpperHandle))
            up = true;
        break;
    case QAbstractSlider::SliderToMaximum:
        value = max;
        if ((main && mainControl == QtRangeSlider::UpperHandle) || (!main && altControl == QtRangeSlider::UpperHandle))
            up = true;
        break;
    case QAbstractSlider::SliderMove:
        if ((main && mainControl == QtRangeSlider::UpperHandle) || (!main && altControl == QtRangeSlider::UpperHandle))
            up = true;
    case QAbstractSlider::SliderNoAction:
        no = true;
        break;
    default:
        qWarning("QtRangeSlider::triggerAction: Unknown action");
        break;
    }

    if (!no && !up)
    {
        if (movement == QtRangeSlider::NoCrossing)
            value = qMin(value, upper);
        else if (movement == QtRangeSlider::NoOverlapping)
            value = qMin(value, upper - 1);

        if (movement == QtRangeSlider::FreeMovement && value > upper)
        {
            swapControls();
            this->setUpperPosition(value);
        }
        else
        {
            this->setLowerPosition(value);
        }
    }
    else if (!no)
    {
        if (movement == QtRangeSlider::NoCrossing)
            value = qMax(value, lower);
        else if (movement == QtRangeSlider::NoOverlapping)
            value = qMax(value, lower + 1);

        if (movement == QtRangeSlider::FreeMovement && value < lower)
        {
            swapControls();
            this->setLowerPosition(value);
        }
        else
        {
            this->setUpperPosition(value);
        }
    }

    blockTracking = false;
    this->setLowerValue(lowerPos);
    this->setUpperValue(upperPos);
}

void QtRangeSlider::swapControls()
{
    qSwap(lower, upper);
    qSwap(lowerPressed, upperPressed);
    lastPressed = (lastPressed == QtRangeSlider::LowerHandle ? QtRangeSlider::UpperHandle : QtRangeSlider::LowerHandle);
    mainControl = (mainControl == QtRangeSlider::LowerHandle ? QtRangeSlider::UpperHandle : QtRangeSlider::LowerHandle);
}

void QtRangeSlider::updateRange(int min, int max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    // setSpan() takes care of keeping span in range
    this->setSpan(lower, upper);
}

void QtRangeSlider::movePressedHandle()
{
    switch (lastPressed)
    {
        case QtRangeSlider::LowerHandle:
            if (lowerPos != lower)
            {
                bool main = (mainControl == QtRangeSlider::LowerHandle);
                triggerAction(QAbstractSlider::SliderMove, main);
            }
            break;
        case QtRangeSlider::UpperHandle:
            if (upperPos != upper)
            {
                bool main = (mainControl == QtRangeSlider::UpperHandle);
                triggerAction(QAbstractSlider::SliderMove, main);
            }
            break;
        default:
            break;
    }
}

/*!
    \class QtRangeSlider
    \inmodule QxtWidgets
    \brief The QtRangeSlider widget is a QSlider with two handles.

    QtRangeSlider is a slider with two handles. QtRangeSlider is
    handy for letting user to choose an span between min/max.

    The span color is calculated based on QPalette::Highlight.

    The keys are bound according to the following table:
    \table
    \header \o Orientation    \o Key           \o Handle
    \row    \o Qt::Horizontal \o Qt::Key_Left  \o lower
    \row    \o Qt::Horizontal \o Qt::Key_Right \o lower
    \row    \o Qt::Horizontal \o Qt::Key_Up    \o upper
    \row    \o Qt::Horizontal \o Qt::Key_Down  \o upper
    \row    \o Qt::Vertical   \o Qt::Key_Up    \o lower
    \row    \o Qt::Vertical   \o Qt::Key_Down  \o lower
    \row    \o Qt::Vertical   \o Qt::Key_Left  \o upper
    \row    \o Qt::Vertical   \o Qt::Key_Right \o upper
    \endtable

    Keys are bound by the time the slider is created. A key is bound
    to same handle for the lifetime of the slider. So even if the handle
    representation might change from lower to upper, the same key binding
    remains.

    \image qxtspanslider.png "QtRangeSlider in Plastique style."

    \bold {Note:} QtRangeSlider inherits QSlider for implementation specific
    reasons. Adjusting any single handle specific properties like
    \list
    \o QAbstractSlider::sliderPosition
    \o QAbstractSlider::value
    \endlist
    has no effect. However, all slider specific properties like
    \list
    \o QAbstractSlider::invertedAppearance
    \o QAbstractSlider::invertedControls
    \o QAbstractSlider::minimum
    \o QAbstractSlider::maximum
    \o QAbstractSlider::orientation
    \o QAbstractSlider::pageStep
    \o QAbstractSlider::singleStep
    \o QSlider::tickInterval
    \o QSlider::tickPosition
    \endlist
    are taken into consideration.
 */

/*!
    \enum QtRangeSlider::HandleMovementMode

    This enum describes the available handle movement modes.

    \value FreeMovement The handles can be moved freely.
    \value NoCrossing The handles cannot cross, but they can still overlap each other. The lower and upper values can be the same.
    \value NoOverlapping The handles cannot overlap each other. The lower and upper values cannot be the same.
 */

/*!
    \enum QtRangeSlider::SpanHandle

    This enum describes the available span handles.

    \omitvalue NoHandle \omit Internal only (for now). \endomit
    \value LowerHandle The lower boundary handle.
    \value UpperHandle The upper boundary handle.
 */

/*!
    \fn QtRangeSlider::lowerValueChanged(int lower)

    This signal is emitted whenever the \a lower value has changed.
 */

/*!
    \fn QtRangeSlider::upperValueChanged(int upper)

    This signal is emitted whenever the \a upper value has changed.
 */

/*!
    \fn QtRangeSlider::spanChanged(int lower, int upper)

    This signal is emitted whenever both the \a lower and the \a upper
    values have changed ie. the span has changed.
 */

/*!
    \fn QtRangeSlider::lowerPositionChanged(int lower)

    This signal is emitted whenever the \a lower position has changed.
 */

/*!
    \fn QtRangeSlider::upperPositionChanged(int upper)

    This signal is emitted whenever the \a upper position has changed.
 */

/*!
    \fn QtRangeSlider::sliderPressed(SpanHandle handle)

    This signal is emitted whenever the \a handle has been pressed.
 */

/*!
    Constructs a new QtRangeSlider with \a parent.
 */
QtRangeSlider::QtRangeSlider(QWidget* parent)
: QSlider(parent),
lower( 0 ),
upper( 0 ),
lowerPos( 0 ),
upperPos( 0 ),
offset( 0 ),
position( 0 ),
lastPressed( QtRangeSlider::NoHandle ),
mainControl( QtRangeSlider::LowerHandle ),
lowerPressed( QStyle::SC_None ),
upperPressed( QStyle::SC_None ),
movement( QtRangeSlider::FreeMovement ),
firstMovement( false ),
blockTracking( false )
{
    connect(this, SIGNAL(rangeChanged(int, int)), this, SLOT(updateRange(int, int)));
    connect(this, SIGNAL(sliderReleased()), this, SLOT(movePressedHandle()));
}

/*!
    Constructs a new QtRangeSlider with \a orientation and \a parent.
 */
QtRangeSlider::QtRangeSlider(Qt::Orientation orientation, QWidget* parent)
: QSlider(orientation, parent),
lower( 0 ),
upper( 0 ),
lowerPos( 0 ),
upperPos( 0 ),
offset( 0 ),
position( 0 ),
lastPressed( QtRangeSlider::NoHandle ),
mainControl( QtRangeSlider::LowerHandle ),
lowerPressed( QStyle::SC_None ),
upperPressed( QStyle::SC_None ),
movement( QtRangeSlider::FreeMovement ),
firstMovement( false ),
blockTracking( false )
{
    connect(this, SIGNAL(rangeChanged(int, int)), this, SLOT(updateRange(int, int)));
    connect(this, SIGNAL(sliderReleased()), this, SLOT(movePressedHandle()));
}

/*!
    Destructs the span slider.
 */
QtRangeSlider::~QtRangeSlider()
{
}

/*!
    \property QtRangeSlider::handleMovementMode
    \brief the handle movement mode
 */
QtRangeSlider::HandleMovementMode QtRangeSlider::handleMovementMode() const
{
    return this->movement;
}

void QtRangeSlider::setHandleMovementMode(QtRangeSlider::HandleMovementMode mode)
{
    this->movement = mode;
}

/*!
    \property QtRangeSlider::lowerValue
    \brief the lower value of the span
 */
int QtRangeSlider::lowerValue() const
{
    return qMin(this->lower, this->upper);
}

void QtRangeSlider::setLowerValue(int lower)
{
    setSpan(lower, this->upper);
}

/*!
    \property QtRangeSlider::upperValue
    \brief the upper value of the span
 */
int QtRangeSlider::upperValue() const
{
    return qMax(this->lower, this->upper);
}

void QtRangeSlider::setUpperValue(int upper)
{
    setSpan(this->lower, upper);
}

/*!
    Sets the span from \a lower to \a upper.
 */
void QtRangeSlider::setSpan(int lower, int upper)
{
    const int low = qBound(minimum(), qMin(lower, upper), maximum());
    const int upp = qBound(minimum(), qMax(lower, upper), maximum());
    if (low != this->lower || upp != this->upper)
    {
        if (low != this->lower)
        {
            this->lower = low;
            this->lowerPos = low;
            emit lowerValueChanged(low);
        }
        if (upp != this->upper)
        {
            this->upper = upp;
            this->upperPos = upp;
            emit upperValueChanged(upp);
        }
        emit spanChanged(this->lower, this->upper);
        update();
    }
}

/*!
    \property QtRangeSlider::lowerPosition
    \brief the lower position of the span
 */
int QtRangeSlider::lowerPosition() const
{
    return this->lowerPos;
}

void QtRangeSlider::setLowerPosition(int lower)
{
    if (this->lowerPos != lower)
    {
        this->lowerPos = lower;
        if (!hasTracking())
            update();
        if (isSliderDown())
            emit lowerPositionChanged(lower);
        if (hasTracking() && !this->blockTracking)
        {
            bool main = (this->mainControl == QtRangeSlider::LowerHandle);
            this->triggerAction(SliderMove, main);
        }
    }
}

/*!
    \property QtRangeSlider::upperPosition
    \brief the upper position of the span
 */
int QtRangeSlider::upperPosition() const
{
    return this->upperPos;
}

void QtRangeSlider::setUpperPosition(int upper)
{
    if (this->upperPos != upper)
    {
        this->upperPos = upper;
        if (!hasTracking())
            update();
        if (isSliderDown())
            emit upperPositionChanged(upper);
        if (hasTracking() && !this->blockTracking)
        {
            bool main = (this->mainControl == QtRangeSlider::UpperHandle);
            this->triggerAction(SliderMove, main);
        }
    }
}

/*!
    \reimp
 */
void QtRangeSlider::keyPressEvent(QKeyEvent* event)
{
    QSlider::keyPressEvent(event);

    bool main = true;
    SliderAction action = SliderNoAction;
    switch (event->key())
    {
    case Qt::Key_Left:
        main   = (orientation() == Qt::Horizontal);
        action = !invertedAppearance() ? SliderSingleStepSub : SliderSingleStepAdd;
        break;
    case Qt::Key_Right:
        main   = (orientation() == Qt::Horizontal);
        action = !invertedAppearance() ? SliderSingleStepAdd : SliderSingleStepSub;
        break;
    case Qt::Key_Up:
        main   = (orientation() == Qt::Vertical);
        action = invertedControls() ? SliderSingleStepSub : SliderSingleStepAdd;
        break;
    case Qt::Key_Down:
        main   = (orientation() == Qt::Vertical);
        action = invertedControls() ? SliderSingleStepAdd : SliderSingleStepSub;
        break;
    case Qt::Key_Home:
        main   = (this->mainControl == QtRangeSlider::LowerHandle);
        action = SliderToMinimum;
        break;
    case Qt::Key_End:
        main   = (this->mainControl == QtRangeSlider::UpperHandle);
        action = SliderToMaximum;
        break;
    default:
        event->ignore();
        break;
    }

    if (action)
        this->triggerAction(action, main);
}

/*!
    \reimp
 */
void QtRangeSlider::mousePressEvent(QMouseEvent* event)
{
    if (minimum() == maximum() || (event->buttons() ^ event->button()))
    {
        event->ignore();
        return;
    }

    this->handleMousePress(event->pos(), this->upperPressed, this->upper, QtRangeSlider::UpperHandle);
    if (this->upperPressed != QStyle::SC_SliderHandle)
        this->handleMousePress(event->pos(), this->lowerPressed, this->lower, QtRangeSlider::LowerHandle);

    this->firstMovement = true;
    event->accept();
}

/*!
    \reimp
 */
void QtRangeSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (this->lowerPressed != QStyle::SC_SliderHandle && this->upperPressed != QStyle::SC_SliderHandle)
    {
        event->ignore();
        return;
    }

    QStyleOptionSlider opt;
    this->initStyleOption(&opt);
    const int m = style()->pixelMetric(QStyle::PM_MaximumDragDistance, &opt, this);
    int newPosition = this->pixelPosToRangeValue(this->pick(event->pos()) - this->offset);
    if (m >= 0)
    {
        const QRect r = rect().adjusted(-m, -m, m, m);
        if (!r.contains(event->pos()))
        {
            newPosition = this->position;
        }
    }

    // pick the preferred handle on the first movement
    if (this->firstMovement)
    {
        if (this->lower == this->upper)
        {
            if (newPosition < lowerValue())
            {
                this->swapControls();
                this->firstMovement = false;
            }
        }
        else
        {
            this->firstMovement = false;
        }
    }

    if (this->lowerPressed == QStyle::SC_SliderHandle)
    {
        if (this->movement == NoCrossing)
            newPosition = qMin(newPosition, upperValue());
        else if (this->movement == NoOverlapping)
            newPosition = qMin(newPosition, upperValue() - 1);

        if (this->movement == FreeMovement && newPosition > this->upper)
        {
            this->swapControls();
            setUpperPosition(newPosition);
        }
        else
        {
            setLowerPosition(newPosition);
        }
    }
    else if (this->upperPressed == QStyle::SC_SliderHandle)
    {
        if (this->movement == NoCrossing)
            newPosition = qMax(newPosition, lowerValue());
        else if (this->movement == NoOverlapping)
            newPosition = qMax(newPosition, lowerValue() + 1);

        if (this->movement == FreeMovement && newPosition < this->lower)
        {
            this->swapControls();
            setLowerPosition(newPosition);
        }
        else
        {
            setUpperPosition(newPosition);
        }
    }
    event->accept();
}

/*!
    \reimp
 */
void QtRangeSlider::mouseReleaseEvent(QMouseEvent* event)
{
    QSlider::mouseReleaseEvent(event);
    setSliderDown(false);
    this->lowerPressed = QStyle::SC_None;
    this->upperPressed = QStyle::SC_None;
    update();
}

/*!
    \reimp
 */
void QtRangeSlider::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QStylePainter painter(this);

    // groove & ticks
    QStyleOptionSlider opt;
    this->initStyleOption(&opt);
    opt.sliderValue = 0;
    opt.sliderPosition = 0;
    opt.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderTickmarks;
    painter.drawComplexControl(QStyle::CC_Slider, opt);

    // handle rects
    opt.sliderPosition = this->lowerPos;
    const QRect lr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    const int lrv  = this->pick(lr.center());
    opt.sliderPosition = this->upperPos;
    const QRect ur = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    const int urv  = this->pick(ur.center());

    // span
    const int minv = qMin(lrv, urv);
    const int maxv = qMax(lrv, urv);
    const QPoint c = QRect(lr.center(), ur.center()).center();
    QRect spanRect;
    if (orientation() == Qt::Horizontal)
        spanRect = QRect(QPoint(minv, c.y() - 2), QPoint(maxv, c.y() + 1));
    else
        spanRect = QRect(QPoint(c.x() - 2, minv), QPoint(c.x() + 1, maxv));
    this->drawSpan(&painter, spanRect);

    // handles
    switch (this->lastPressed)
    {
    case QtRangeSlider::LowerHandle:
        this->drawHandle(&painter, QtRangeSlider::UpperHandle);
        this->drawHandle(&painter, QtRangeSlider::LowerHandle);
        break;
    case QtRangeSlider::UpperHandle:
    default:
        this->drawHandle(&painter, QtRangeSlider::LowerHandle);
        this->drawHandle(&painter, QtRangeSlider::UpperHandle);
        break;
    }
}
