#ifndef NEWRESOURCEDIALOG_H
#define NEWRESOURCEDIALOG_H

#include <QDialog>

namespace Ui {
class NewResourceDialog;
}

class NewResourceDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit NewResourceDialog(QWidget *parent = 0);
    ~NewResourceDialog();
    
private:
    Ui::NewResourceDialog *ui;
};

#endif // NEWRESOURCEDIALOG_H
