#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QTextStream>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QStringListModel>

#include "NewResourceDialog.h"
#include "CodeEditor/codeEditor.h"
#include "CodeEditor/highlighter.h"
#include "ConsoleHighlighter.h"

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
    connect(ui->actionNew_Resource, SIGNAL(triggered()), this, SLOT(newResource()));
    connect(ui->actionShow_Console, SIGNAL(triggered()), this, SLOT(showConsole()));
    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(runGame()));
    
    createCodeEditor();
    
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
    
    new ConsoleHighlighter(ui->console->document()); 
    
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

void MainWindow::createCodeEditor() {
    CodeEditor *codeEditor = new CodeEditor("Untitled.as", this);
    codeEditor->setUndoRedoEnabled(true);
    codeEditor->setTabStopWidth(29);
    
#ifdef Q_OS_WIN
    int size = 10;
    QFont font("Consolas", size);
#endif
    
#ifdef Q_OS_MAC
    int size = 12;
    QFont font("Monaco", size);
#endif
    
    codeEditor->setFont(font);
    new Highlighter(codeEditor->document());
    mCompleter = new QCompleter();
    
    mCompleter->setModel(modelFromFile(":/Resources/AutocompleteWords.txt"));
    mCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    mCompleter->setWrapAround(false);
    mCompleter->popup()->setStyleSheet("color: #848484;"
                                       "background-color: #2E2E2E;"
                                       "selection-background-color: #424242;"
                                       "alternate-background-color: #292929;");
    mCompleter->popup()->setAlternatingRowColors(true);
    codeEditor->setCompleter(mCompleter);
    
    connect(codeEditor, SIGNAL(cursorPosition(int)), SLOT(getCursorPosition(int)));
    connect(codeEditor, SIGNAL(pressed(QString)), this, SLOT(keyPressed(QString)));
    
    mCentralWidget->addWidget(codeEditor);
    codeEditor->setFocus();
    codeEditor->updateCorner();
}

QAbstractItemModel *MainWindow::modelFromFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "no open is a sad";
        return new QStringListModel(mCompleter);
    }

    QStringList words;
    
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty()) {
            words << line.trimmed();
        }
    }
    
    words.sort();
    
    return new QStringListModel(words, mCompleter);
}

void MainWindow::open() {
    qDebug() << "path" << QDir::homePath() + "/Documents/";
    QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath() + "/Documents/");
}

void MainWindow::save() {
    
}

void MainWindow::saveAs() {
    
}

void MainWindow::newResource() {
    NewResourceDialog *nd = new NewResourceDialog(this);
    nd->exec();
}

void MainWindow::showConsole() {
    ui->consoleDockWidget->setHidden(!ui->consoleDockWidget->isHidden());
}

void MainWindow::updateConsoleSTDOut() {
    updateConsole(mPlayer->readAllStandardOutput());
}

void MainWindow::updateConsoleSTDErr() {
    updateConsole(mPlayer->readAllStandardError());
}

void MainWindow::updateConsole(const QString &str) {
    ui->console->append(str);
}

void MainWindow::MainWindow::runGame() {
    ui->console->clear();
    ui->consoleDockWidget->show();
    
    mPlayer = new QProcess(this);
    connect(mPlayer, SIGNAL(readyReadStandardOutput()), this, SLOT(updateConsoleSTDOut()));
    connect(mPlayer, SIGNAL(readyReadStandardError()), this, SLOT(updateConsoleSTDErr()));
    mPlayer->start("/Users/Skyler/Documents/UrhoEdit/UrhoEdit/Resources/Player/Urho3DPlayer", QStringList() << "/Users/Skyler/Downloads/Urho3D-master/Bin/Data/Scripts/19_VehicleDemo.as");
}
