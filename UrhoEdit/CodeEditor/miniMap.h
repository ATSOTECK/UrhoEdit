#ifndef MINIMAPC_H
#define MINIMAPC_H

#include <QFrame>
#include <QPlainTextEdit>
#include <QTextOption>
#include <QGraphicsOpacityEffect>
#include <QFontMetrics>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <Qt>
#include <QPropertyAnimation>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>

class Highlighter;
class CodeEditor;
class MiniMapC;

class SliderArea : public QFrame {
public:
    friend class MiniMapC;
    
    SliderArea(QFrame *parent = 0, MiniMapC *m = 0);

    void updatePosition();

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);

    void moveSlider(float y);

    bool isPressed() const {
        return mPressed;
    }

    void setLinesCount(int lines);

private:
    QFrame *mParent;
    QColor mColor;
    QGraphicsOpacityEffect *mGoe;
    bool mPressed;
    int mScrollMargins[2];
    int mCountLines;

    bool mac;
    
    MiniMapC *mMini;
};

class MiniMapC : public QPlainTextEdit {
public:
    MiniMapC(QPlainTextEdit *parent = 0, Highlighter *h = 0, CodeEditor *c = 0);
    ~MiniMapC();

    void calculateMax();

    void wheelEvent(QWheelEvent *e);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);
    
    void sliderAreaWheelEvent(QWheelEvent *e);

    void setCode(const QString &source);

    void adjustToParent();

    void scrollArea(QPointF parentPos, QPointF sliderPos);

    void updateVisibleArea();

    void setSliderAreaLinesCount();
    
    void keyEvent(QKeyEvent *e); 

private:
    void keyPressEvent(QKeyEvent *e);
    
    int mLinesCount;
    QPlainTextEdit *mParent;
    CodeEditor *mCodeEditor;
    QGraphicsOpacityEffect *mGoe;
    QPropertyAnimation *mAnimation;
    SliderArea *mSlider;
    Highlighter *mHighlighter;
    QTextCursor mCursor;

    bool mac;
};

#endif // MINIMAPC_H
