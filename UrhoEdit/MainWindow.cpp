#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    setupUi();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUi() {
    //Don't show that annoying blue rectangle
    ui->resourcesTree->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->resourcesTree->setHeaderHidden(true);
    
    ui->resourcesDockWidgetContents->layout()->setContentsMargins(0, 0, 0, 0);
    
    
    QFile *style;
    QString styleText;
    
#ifdef Q_OS_MAC
    style = new QFile(":/Styles/DarkMainMac.qss");
#endif
    
    if (style->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(style);
        styleText = in.readAll();
    }
    style->close();
    delete style;
    setStyleSheet(styleText);
}
