#include "waitdialog.h"
#include "ui_waitdialog.h"


waitDialog::waitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::waitDialog)
{
    ui->setupUi(this);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

waitDialog::~waitDialog()
{
    delete ui;
}
