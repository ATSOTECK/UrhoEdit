#include "searchWidget.h"

#include <QStyle>
#include <QString>

#include <QDebug>

LineEditCount::LineEditCount(LineEdit *edit) //:
    //QObject(this)
{
    QHBoxLayout *layout = new QHBoxLayout(edit);
    layout->setMargin(0);
    edit->setLayout(layout);
    layout->addStretch();
    mCounter = new QLabel(edit);
    layout->addWidget(mCounter);
    edit->setStyleSheet("padding-right: 2px;");
    edit->setTextMargins(0, 0, 60, 0);
}

void LineEditCount::updateCount(int index, int total, bool hasSearch) {
    QString message = tr("%1 of %2").arg(index).arg(total);
    mCounter->setText(message);
    mCounter->setStyleSheet("background: none;color: gray;");
    if (index == 0 && total == 0 && hasSearch) {
        mCounter->setStyleSheet("background: #e73e3e;color: white;border-radius: 5px;");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

LineEdit::LineEdit(SearchWidget *parent) :
    QLineEdit(parent),
    mParent(parent)
{
    mCounter = new LineEditCount(this);
    setAttribute(Qt::WA_MacShowFocusRect, false);
}

void LineEdit::keyPressEvent(QKeyEvent *e) {
    
    switch (e->key()) {
    case Qt::Key_Control:
    case Qt::Key_Shift:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Down:
    case Qt::Key_Up:
    case Qt::Key_Alt:
        QLineEdit::keyPressEvent(e);
        return;
    default:
        break;
    }
    
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        mParent->findNext();
        return;
    }
    
    QLineEdit::keyPressEvent(e);
    mParent->findMatches();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

SearchWidget::SearchWidget(CodeEditor *editor, QWidget *parent) :
    QWidget(parent),
    mTotalMatches(0),
    mIndex(0),
    mCodeEditor(editor)
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    mCheckSensitive = new QCheckBox(tr("Case Sensitive"), this);
    mCheckWholeWord = new QCheckBox(tr("Find Whole Words"), this);
    mFindText = new LineEdit(this);
    mFindText->setMinimumWidth(250);
    mFindText->setPlaceholderText(" Find");
    QIcon closeIcon = QIcon(":/Images/16x16/close.png");
    //QIcon findIcon = QIcon(":/Images/32x32/find.png");
    mButtonClose = new QPushButton(closeIcon, "", this);
    mButtonClose->setToolTip(tr("Close"));
    //mButtonFind = new QPushButton(findIcon, "", this);
    //mButtonFind->setToolTip(tr("Find"));
    mButtonPrevious = new QPushButton(style()->standardIcon(QStyle::SP_ArrowLeft), "", this);
    mButtonPrevious->setToolTip(tr("Find Previous"));
    mButtonNext = new QPushButton(style()->standardIcon(QStyle::SP_ArrowRight), "", this);
    mButtonNext->setToolTip(tr("Find Next"));
    
    layout->addWidget(mButtonClose);
    layout->addWidget(mFindText);
    //layout->addWidget(mButtonFind);
    layout->addWidget(mButtonPrevious);
    layout->addWidget(mButtonNext);
    layout->addWidget(mCheckSensitive);
    layout->addWidget(mCheckWholeWord);
    
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(mButtonNext, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(mButtonPrevious, SIGNAL(clicked()), this, SLOT(findPrevious()));
    
    connect(mCheckSensitive, SIGNAL(stateChanged(int)), this, SLOT(findMatches()));
    connect(mCheckWholeWord, SIGNAL(stateChanged(int)), this, SLOT(findMatches()));
    
    mFindText->counter()->updateCount(mIndex, mTotalMatches);
}

SearchWidget::~SearchWidget() {
    
}

void SearchWidget::setCodeEditor(CodeEditor *editor) {
    mCodeEditor = editor;
    findMatches();
}

void SearchWidget::closeClicked() {
    emit closeButtonClicked();
}

void SearchWidget::findNext() {
    mCodeEditor->findMatch(mFindText->text(), true, mCheckSensitive->isChecked(), 
                           mCheckWholeWord->isChecked(), 0);
    if (mTotalMatches > 0 && mIndex < mTotalMatches) {
        mIndex += 1;
    } else if (mTotalMatches > 0) {
        mIndex = 1;
    }
    mFindText->counter()->updateCount(mIndex, mTotalMatches);
}

void SearchWidget::findPrevious() {
    mCodeEditor->findMatch(mFindText->text(), true, mCheckSensitive->isChecked(), 
                           mCheckWholeWord->isChecked(), 1);
    if (mTotalMatches > 0 && mIndex > 1) {
        mIndex -= 1;
    } else if (mTotalMatches > 0) {
        mIndex = mTotalMatches;
        mCodeEditor->moveCursor(QTextCursor::End);
        mCodeEditor->findMatch(mFindText->text(), true, mCheckSensitive->isChecked(), 
                               mCheckWholeWord->isChecked(), 1);
    }
    mFindText->counter()->updateCount(mIndex, mTotalMatches);
}

void SearchWidget::findMatches() {
    QString text = mCodeEditor->toPlainText();
    QString search = mFindText->text();
    if (search.size() == 0) 
        return;
    
    QRegExp r;
    
    if (mCheckWholeWord->isChecked()) {
        QString pat;
        
        if (mCheckSensitive->isChecked()) {
            pat = QString("\\b" + search.toLower() + "\\b");
        } else {
            pat = QString("\\b" + search + "\\b"); 
        }
        r = QRegExp(pat);
        mTotalMatches = text.count(r);
    }
    
    if (mCheckSensitive->isChecked() && !mCheckWholeWord->isChecked()) {
        mTotalMatches = text.count(search);
    }
    
    if (!mCheckSensitive->isChecked() && !mCheckWholeWord->isChecked()) {
        mTotalMatches = text.toLower().count(search.toLower());
    }
    
    if (mTotalMatches > 0) {
        QTextCursor cursor = mCodeEditor->textCursor();
        cursor.movePosition(QTextCursor::WordLeft);
        cursor.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);
        text = cursor.selectedText();
        mIndex = text.count(search) + 1;
    } else {
        mIndex = 0;
    }
    
    mFindText->counter()->updateCount(mIndex, mTotalMatches);
    
    mCodeEditor->findMatch(mFindText->text(), false, mCheckSensitive->isChecked(), 
                           mCheckWholeWord->isChecked());
}
