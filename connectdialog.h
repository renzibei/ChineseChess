#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

private:
    Ui::ConnectDialog *ui;
private slots:
    void cancelButtonPushed();
    void okButtonPushed();

};

#endif // CONNECTDIALOG_H
