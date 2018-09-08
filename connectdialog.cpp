#include "connectdialog.h"
#include "ui_connectdialog.h"
#include "gamecenter.h"
#include <QMessageBox>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okButtonPushed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPushed()));
}



bool isLegalIp(QString ipAddress)
{
    QHostAddress test;
    if (!test.setAddress(ipAddress))
    {
        //ui->statusBar->showMessage("ERROR : Invalid ip address.");
        return false;
    }
    return true;

}

void ConnectDialog::okButtonPushed()
{
    QString ipString = ui->lineEdit->text();
    if(isLegalIp(ipString)) {
        GameCenter::getInstance()->netServer->setHostIp(ipString);
        GameCenter::getInstance()->netServer->connectServer();
        this->close();
    }
    else QMessageBox::information(this, tr("错误"), tr("ip地址格式错误"));
}

void ConnectDialog::cancelButtonPushed()
{
    this->close();
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}
