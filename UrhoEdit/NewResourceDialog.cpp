#include "NewResourceDialog.h"
#include "ui_NewResourceDialog.h"

NewResourceDialog::NewResourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewResourceDialog)
{
    ui->setupUi(this);
}

NewResourceDialog::~NewResourceDialog() {
    delete ui;
}
