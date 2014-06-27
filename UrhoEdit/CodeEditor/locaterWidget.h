#ifndef LOCATERWIDGET_H
#define LOCATERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>

class LocaterWidget : public QWidget {
public:
    LocaterWidget(QWidget *parent = 0);
    
private:
    QVBoxLayout *mLayout;
    QTextEdit *mTxt;
};

#endif // LOCATERWIDGET_H
