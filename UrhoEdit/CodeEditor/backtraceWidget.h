#ifndef BACKTRACEWIDGET_H
#define BACKTRACEWIDGET_H

#include <QWidget>
#include <QListView>
#include <QVBoxLayout>

class BacktraceWidget : public QWidget {
public:
    BacktraceWidget(QWidget *parent = 0);
    
private:
    QVBoxLayout *mLayout;
    QListView *mView;
};

#endif // BACKTRACEWIDGET_H
