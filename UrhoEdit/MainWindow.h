#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QProcess>
#include <QCompleter>
#include <QMainWindow>
#include <QStackedWidget>
#include <QAbstractItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void debug(const QString &msg);
    void warn(const QString &msg);
    void crit(const QString &msg);
    void fail(const QString &msg);
    
    void setupUi();
    
    void createCodeEditor();
    
    QAbstractItemModel *modelFromFile(const QString &fileName);
    
public slots:
    void open();
    void save();
    void saveAs();
    
    void newResource();
    
    void showConsole();
    
    void updateConsoleSTDOut();
    void updateConsoleSTDErr();
    void updateConsole(const QString &str);
    
    void runGame();
    
private:
    Ui::MainWindow *ui;
    
    QStackedWidget *mCentralWidget;
    
    QCompleter *mCompleter;
    
    QProcess *mPlayer;
};

void setMainWindow(MainWindow *mw);
void catchMessage(QtMsgType type, const QMessageLogContext &, const QString &msg);

#endif // MAINWINDOW_H
