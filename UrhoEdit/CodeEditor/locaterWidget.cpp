#include "locaterWidget.h"

LocaterWidget::LocaterWidget(QWidget *parent) :
    QWidget(parent),
    mLayout(new QVBoxLayout(this)),
    mTxt(new QTextEdit(this))
{
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->addWidget(mTxt);
    
    mTxt->setText("This is just a placeholder.");
    mTxt->setFrameStyle(QFrame::NoFrame);
}
