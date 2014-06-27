#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QTextStream>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

MainWindow *w;

void setMainWindow(MainWindow *mw) {
    w = mw;
}

void catchMessage(QtMsgType type, const QMessageLogContext &, const QString &msg) {
    switch (type) {
    case QtDebugMsg:
        w->debug(msg);
        break;
    case QtWarningMsg:
        w->warn(msg);
        break;
    case QtCriticalMsg:
        w->crit(msg);
        break;
    case QtFatalMsg:
        w->fail(msg);
        break;
    default:
        break;
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mCentralWidget(new QStackedWidget(this))
{
    ui->setupUi(this);
    
    setupUi();
    
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(runGame()));
    
    qInstallMessageHandler(catchMessage);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::debug(const QString &msg) {
    QColor c(98, 217, 239);
    ui->console->setTextColor(c);
    ui->console->append(msg);
    ui->console->setTextColor(Qt::white);
}
void MainWindow::warn(const QString &msg) {
    ui->console->setTextColor(QColor(230, 219, 116));
    ui->console->append(msg);
    ui->console->setTextColor(Qt::white);
}

void MainWindow::crit(const QString &msg) {
    ui->console->setTextColor(Qt::red);
    ui->console->append(msg);
    ui->console->setTextColor(Qt::white);
}

void MainWindow::fail(const QString &msg) {
    ui->console->setTextColor(Qt::red);
    ui->console->append(msg);
    ui->console->setTextColor(Qt::white);
}

void MainWindow::setupUi() {
    setCentralWidget(mCentralWidget);
    
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    
    //Don't show that annoying blue rectangle on mac
    ui->resourcesTree->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->resourcesTree->setHeaderHidden(true);
    
    ui->resourcesDockWidgetContents->layout()->setContentsMargins(0, 0, 0, 0);
    ui->consoleDockWidgetContents->layout()->setContentsMargins(0, 0, 0, 0);
    
    
    QIcon runIcon(":/Resources/Images/run.png");
    QAction *actionRun = new QAction(runIcon, tr("Run"), this);
    
    connect(actionRun, SIGNAL(triggered()), this, SLOT(runGame()));
    
    ui->mainToolBar->addAction(actionRun);
    
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

void MainWindow::open() {
    qDebug() << "path" << QDir::homePath() + "/Documents/";
    QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath() + "/Documents/");
}

void MainWindow::save() {
    
}

void MainWindow::saveAs() {
    
}

void MainWindow::runGame() {
    
}
