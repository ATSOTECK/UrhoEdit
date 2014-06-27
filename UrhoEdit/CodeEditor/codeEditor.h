#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QTextDocumentWriter>
#include <QObject>
#include <QCompleter>
#include <QList>
#include <QWheelEvent>
#include <QWidget>

#include "bookMark.h"
#include "highlighter.h"
#include "miniMap.h"
#include "addRemoveText.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QCompleter;
class QTimer;
class QUndoStack;

class LineNumberArea;
class Highlighter;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QString name, QWidget *parent = 0, Highlighter *h = 0);
    ~CodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    //void mousePressEvent(QMouseEvent *e);

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

    int foldArea;
    QVector<int> breakPoints;
    QVector<int> bookmarks;
    QList<int> mFoldedLines;
    QList<QTextBlock> *mBookmarks;

    //protected in QPlainTextEdit
    QTextBlock editorFirstVisibleBlock();
    int editorFirstVisibleBlockNumber();
    QPointF editorContentOffset();
    QRectF editorBlockBoundingGeometry(const QTextBlock &block) const;

    QString getName() const {
        return mName;
    }
    
    bool isModified() {
        return true;
    }

    bool saveToFile(QString &path);
    bool openFile(const QString &path);
    
    void lineNumberAreaMousePressEvent(QMouseEvent *e);
    void lineNumberAreaMouseMoveEvent(QMouseEvent *e);
    void lineNumberAreaWheelEvent(QWheelEvent *e);
    
    void findMatch(QString text, bool next = false, bool caseSensitive = false, bool wholeWord = false, int foo = 5);
    
    QUndoStack *undoStack() {
        return mUndoStack;
    }
    
    bool insideComment(int lineNumber);
    
#ifdef Q_OS_WIN
    int round(float x);
#endif

public slots:
    void findText();
    void updateMiniMapText();
    void updateMiniMapVisibleArea();
    void setModified();
    
    void cleanStateChanged(bool state);
    
    //preferences
    void setUseMiniMap(bool mini);
    void setUseTabs(bool tabs);
    void setShowTabsSpaces(bool tabsSpaces);
    void setUseWordWrap(bool wrap);
    
    void updateCorner();

protected:
    void resizeEvent(QResizeEvent *event);

    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    
    void paintEvent(QPaintEvent *e);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void highlightSelectedWord(QString txt = "");
    void updateLineNumberArea(const QRect & rect, int dy);
    void updateMiniMapScrollPos();
    void insertCompletion(const QString &completion);
    void blockOrColumnChanged();
    void highlightJumpToDefinition(QTextCursor &cursor, bool pressed);
    
signals:
    void statusInfoChanged(QString info);
    void locateFunction(QString text, bool);
    
    void modifiedChanged(bool);

private:
    friend class LineNumberArea;
    
    void completeBraces(QString text);
    void completeQuotes(QString text);
    void insertIndentation();
    void indentMore();
    void autoCompleteEnter();
    bool checkNextLine(QString &txt);
    void autoCompleteBackspace();
    void autoCompleteL(QString txt);
    bool isInBPBQ(); //BPBQ Brace, Paran, Braket, Quote
    void setUpMiniMap();
    void tearDownMiniMap();
    void buildVarTable();
    QChar lastNonWhitespaceChar(QString str);
    
    void gotoDefinition(QTextCursor *cursor = 0);
    
    QString getIndentation(const QString &text);
    
    QString reverseSelectTextPortionFromOffset(int begin, int end);
    
    void wheelEvent(QWheelEvent *e);

    QWidget *mLineNumberArea;

    QString textUnderCursor() const;
    QString textLeftOfCursor(QTextCursor cur, bool start = false) const;
    QString textRightOfCursor(QTextCursor cur, bool end  = false) const;
    QString textLeftOfCursor(bool start = false) const;
    QString textRightOfCursor(bool end  = false) const;

    QCompleter *c;

    QString mName;

    QPixmap mRightArrowIcon;
    QPixmap mDownArrowIcon;

    Highlighter *mHighlighter;
    MiniMapC *mMiniMap;

    QTimer *mTimer;
    
    QString mSelectedText;
    
    bool mSet;
    bool mIsModified;
    bool mUseTabs;
    
    QUndoStack *mUndoStack;
    
    QList<QString> mVarTable;
    QList<QTextEdit::ExtraSelection> mExtraSelections;
    bool mInitialSelectionAdded;
    
    QWidget *mCornerWidget;
};

class LineNumberArea : public QWidget {
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }
    
    void codeFoldingEvent(int lineNumber);
    void fold(int lineNumber, bool comment = false);
    void unfold(int lineNumber, bool comment = false);
    void isFolded(int lineNumber);
    
    int findClosing(QTextBlock block, bool comment = false);

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    void wheelEvent(QWheelEvent *event);

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
