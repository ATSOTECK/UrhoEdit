#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QObject>
#include <QHBoxLayout>

#include "codeEditor.h"

class LineEdit;
class SearchWidget;

class LineEditCount : public QObject {
    Q_OBJECT
    
public:
    LineEditCount(LineEdit *edit);
    
    void updateCount(int index, int total, bool hasSearch = false);
    
    QLabel *counter() const {
        return mCounter;
    }
    
private:
    QLabel *mCounter;
    LineEdit *mEdit;
};

class LineEdit : public QLineEdit {
    Q_OBJECT
    
public:
    LineEdit(SearchWidget *parent = 0);
    
    LineEditCount *counter() const {
        return mCounter;
    }
    
protected:
    void keyPressEvent(QKeyEvent *e);
    
private:
    SearchWidget *mParent;
    LineEditCount *mCounter;
};

class SearchWidget : public QWidget {
    Q_OBJECT
    
public:
    SearchWidget(CodeEditor *editor, QWidget *parent = 0);
    ~SearchWidget();
    
    void setCodeEditor(CodeEditor *editor);
    
    LineEdit *lineEdit() const {
        return mFindText;
    }
    
public slots:
    void closeClicked();
    void findNext();
    void findPrevious();
    void findMatches();
    
signals:
    void closeButtonClicked();
    
private:
    int mTotalMatches;
    int mIndex;
    
    QPushButton *mButtonClose;
    LineEdit *mFindText;
    QPushButton *mButtonFind;
    QPushButton *mButtonNext;
    QPushButton *mButtonPrevious;
    QCheckBox *mCheckSensitive;
    QCheckBox *mCheckWholeWord;
    
    CodeEditor *mCodeEditor;
    
    QHBoxLayout *layout;
};

#endif // SEARCHWIDGET_H
