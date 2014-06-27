#include "miniMap.h"
#include "codeEditor.h"

#include <QScrollBar>

MiniMapC::MiniMapC(QPlainTextEdit *parent, Highlighter *h, CodeEditor *c):
    QPlainTextEdit(parent)
{
    setWordWrapMode(QTextOption::NoWrap);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setReadOnly(true);
    //setCenterOnScroll(true);
    setMouseTracking(true);
    viewport()->setCursor(Qt::PointingHandCursor);
    setTextInteractionFlags(Qt::NoTextInteraction);
    //grabKeyboard();

    mParent = parent;
    mLinesCount = 0;
    setSliderAreaLinesCount();
    mHighlighter = h;
    mCodeEditor = c;

#ifdef Q_OS_MAC
    mac = true;
#else
    mac = false;
#endif

    if (!mac) {
        mGoe = new QGraphicsOpacityEffect();
        setGraphicsEffect(mGoe);
        mGoe->setOpacity(0.10f);//min
        mAnimation = new QPropertyAnimation(mGoe, "opacity");
    } else {
        mGoe = 0;
        mAnimation = 0;
    }

    setFrameStyle(0);
    setTabStopWidth(2);

    mSlider = new SliderArea(this, this);
    mSlider->show();
}

MiniMapC::~MiniMapC() {

}

void MiniMapC::calculateMax() {
    int lineHeight = mParent->cursorRect().height();
    if (lineHeight > 0) {
        mLinesCount = mParent->viewport()->height() / lineHeight;
        setSliderAreaLinesCount();
    }
    mSlider->updatePosition();
    updateVisibleArea();
}

void MiniMapC::updateVisibleArea() {
    if (!mSlider->isPressed()) {
        int lineNumber = mCodeEditor->editorFirstVisibleBlockNumber();
        QTextBlock block = document()->findBlockByLineNumber(lineNumber);
        mCursor = textCursor();
        mCursor.setPosition(block.position());
        QRect rect = cursorRect(mCursor);
        setTextCursor(mCursor);
        mSlider->moveSlider(rect.y());
    }
}

void MiniMapC::setCode(const QString &source) {
    setPlainText(source);
    //setDocument(source);
    calculateMax();
}

void MiniMapC::adjustToParent() {
    setFixedHeight(mParent->height());
    setFixedWidth(mParent->width() * (17) / 100);
    int x = mParent->width() - width();
    move(x, 0);
    int fontSize = (int) width() / 80;
    if (fontSize < 1)
        fontSize = 1;
    QFont font = document()->defaultFont();
    font.setPointSize(fontSize);
    font.setStyleStrategy(QFont::PreferAntialias);
    this->setFont(font);
    calculateMax();
}

void MiniMapC::enterEvent(QEvent *) {
    if (mac)
        return;

    mAnimation->setDuration(300);
    float min = 0.10f;
    float max = 0.80f;
    mAnimation->setStartValue(min);//min opacity
    mAnimation->setEndValue(max);//max opacity
    mAnimation->start();
}

void MiniMapC::leaveEvent(QEvent *) {
    if (mac)
        return;

    mAnimation->setDuration(300);
    float min = 0.10f;
    float max = 0.80f;
    mAnimation->setStartValue(max);//max opacity
    mAnimation->setEndValue(min);//min opacity
    mAnimation->start();
}

void MiniMapC::mousePressEvent(QMouseEvent *e) {
    mCursor = cursorForPosition(e->pos());
    QTextCursor c = mParent->textCursor();
    c.setPosition(mCursor.position());
    mParent->setTextCursor(c);
    verticalScrollBar()->setValue(mParent->verticalScrollBar()->value());
}

void MiniMapC::resizeEvent(QResizeEvent *) {
    mSlider->updatePosition();
}

void MiniMapC::scrollArea(QPointF parentPos, QPointF sliderPos) {
    QPointF p = parentPos;
    p.setY(parentPos.y() - sliderPos.y());
    QTextCursor c = cursorForPosition(p.toPoint());
    mParent->verticalScrollBar()->setValue(c.blockNumber());
}

void MiniMapC::wheelEvent(QWheelEvent *e) {
    /*
    int degrees = e->delta() / 8;
    int steps = degrees / 4;
    if (e->orientation() == Qt::Vertical) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() - steps);
        mParent->verticalScrollBar()->setValue(mParent->verticalScrollBar()->value() - steps);
    }
    */
    mCodeEditor->lineNumberAreaWheelEvent(e);
}

void MiniMapC::setSliderAreaLinesCount() {
    mSlider->setLinesCount(mLinesCount);
}

void MiniMapC::keyEvent(QKeyEvent *e) {
    keyPressEvent(e);
}

void MiniMapC::keyPressEvent(QKeyEvent *e) {
    QPlainTextEdit::keyPressEvent(e);
}

void MiniMapC::sliderAreaWheelEvent(QWheelEvent *e) {
    wheelEvent(e);
}

/////////////////////////////////////////////////////////////////////////////////////////

SliderArea::SliderArea(QFrame *parent, MiniMapC *m):
    QFrame(parent),
    mParent(parent),
    mMini(m)
{
    setMouseTracking(true);
    setCursor(Qt::OpenHandCursor);
    mColor = QColor(133, 133, 133, 128);
    setStyleSheet("background: " + mColor.name());

#ifdef Q_OS_MAC
    mac = true;
#else
    mac = false;
#endif

    if (!mac) {
        mGoe = new QGraphicsOpacityEffect();
        setGraphicsEffect(mGoe);
        mGoe->setOpacity(0.80f);
    } else {
        mGoe = 0;
        setStyleSheet("background: transparent;");
    }
    mPressed = false;
    mCountLines = 57;
    mScrollMargins[0] = 0;
    mScrollMargins[1] = 0;
}

void SliderArea::paintEvent(QPaintEvent *event) {
    QPainter *painter = new QPainter();
    painter->begin(this);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->fillRect(event->rect(), QBrush(QColor(255, 255, 255, 80)));
    painter->setPen(QPen(Qt::NoPen));
    painter->end();
}

void SliderArea::updatePosition() {
    int fontSize = QFontMetrics(mParent->font()).height();
    int height = mCountLines * fontSize;
    setFixedHeight(height);
    setFixedWidth(mParent->width());
    mScrollMargins[0] = height;
    mScrollMargins[1] = (mParent->height() - height);
}

void SliderArea::moveSlider(float y) {
    this->move(0, y);
}

void SliderArea::mousePressEvent(QMouseEvent *) {
    mPressed = true;
    setCursor(Qt::ClosedHandCursor);
}

void SliderArea::mouseReleaseEvent(QMouseEvent *) {
    mPressed = false;
    setCursor(Qt::OpenHandCursor);
}

void SliderArea::mouseMoveEvent(QMouseEvent *event) {
    if (mPressed) {
        QPointF pos = mapToParent(event->pos());
        int y = pos.y() - (height() / 2);
        if (y < 0)
            y = 0;
        if (y < mScrollMargins[0]) {
            mMini->verticalScrollBar()->setSliderPosition(mMini->verticalScrollBar()->sliderPosition() - 2);
        } else if (y > mScrollMargins[1]) {
            mMini->verticalScrollBar()->setSliderPosition(mMini->verticalScrollBar()->sliderPosition() + 2);
        }
        moveSlider(y);
        mMini->scrollArea(pos, event->pos());
    }
}

void SliderArea::setLinesCount(int /*lines*/) {
    //mCountLines = lines;
}

void SliderArea::wheelEvent(QWheelEvent *e) {
    mMini->sliderAreaWheelEvent(e);
}
















