#include "backtraceWidget.h"

BacktraceWidget::BacktraceWidget(QWidget *parent) :
    QWidget(parent),
    mLayout(new QVBoxLayout(this)),
    mView(new QListView(this))
{
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->setSpacing(0);
    
    mLayout->addWidget(mView);
    
    mView->setAlternatingRowColors(true);
    mView->setFrameStyle(QFrame::NoFrame);
    mView->setStyleSheet("background: #1C1C1C;");
    mView->setAttribute(Qt::WA_MacShowFocusRect, false);
}
