#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    
public slots:
    void open();
    void save();
    void saveAs();
    
    void runGame();
    
private:
    Ui::MainWindow *ui;
};

void setMainWindow(MainWindow *mw);
void catchMessage(QtMsgType type, const QMessageLogContext &, const QString &msg);

#endif // MAINWINDOW_H
