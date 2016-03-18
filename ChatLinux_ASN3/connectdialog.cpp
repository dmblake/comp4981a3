#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_pushButton_clicked()

{
    QString userString =  ui->usernames->toPlainText();
    QByteArray userArray = userString.toUtf8();
    ((MainWindow*)parent())->setUserName(userArray.data());


    QString ipString =  ui->ipAddress->toPlainText();
    QByteArray ipArray = ipString.toUtf8();
    ((MainWindow*)parent())->setServer(ipArray.data());


    QString portString =  ui->port->toPlainText();
    QByteArray portArray = portString.toUtf8();
    ((MainWindow*)parent())->setPort(portArray.data());
    close();
}
