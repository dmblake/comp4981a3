#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QDebug>
/*-------------------------------------------------------------------------------------------------
 * SOURCE FILE:     connectdialog.cpp - The UI that allows client to connect to server (Dialog)
 *
 * PROGRAM:         ChatLinux_ASN3
 *
 * FUNCTIONS:       ConnectDialog::ConnectDialog(QWidget *parent)
 *                  ConnectDialog::~ConnectDialog()
 *                  void ConnectDialog::on_pushButton_clicked()
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * NOTES:           In order to start chatting with others, you have to first connect to the
 *                  server. The button to connect to the server is located under the Menu
 *                  tab called 'Connect'. Connect button opens a Dialog that has functionality
 *                  in this file.
 *
 *------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
 * FUNCTION         ConnectDialog
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       ConnectDialog()
 *
 * RETURNS:         void
 *
 * NOTES:           This is a constructor for the ConnectDialog class.
 *
 *------------------------------------------------------------------------------------------------*/

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    ui->ipAddress->setText("127.0.0.1");
    ui->port->setText("4981");
    //ui->ipAddress->setPlaceholderText("127.0.0.1");
    //ui->port->setPlaceholderText("4981");
}


/*-------------------------------------------------------------------------------------------------
 * FUNCTION         ~ConnectDialog
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       ~ConnectDialog()
 *
 * RETURNS:         void
 *
 * NOTES:           This is a deconstructor for the ConnectDialog class.
 *
 *------------------------------------------------------------------------------------------------*/

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

/*-------------------------------------------------------------------------------------------------
 * FUNCTION         on_pushButton_clicked
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void on_pushButton_clicked()
 *
 * RETURNS:         void
 *
 * NOTES:           This function allows functionality for a button that sets the username,
 *                  server and port in the main window. Calls function setUserName,
 *                  setServer and setPort from MainWindow class.
 *------------------------------------------------------------------------------------------------*/

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


/*-------------------------------------------------------------------------------------------------
 * FUNCTION         on_fileCheck_clicked
 *
 * DATE:            March 21st 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void on_fileCheck_clicked()
 *
 * RETURNS:         void
 *
 * NOTES:           This function allows functionality to send over if client has checked for
 *                  a file dump or not.
 *------------------------------------------------------------------------------------------------*/

void ConnectDialog::on_fileCheck_clicked(bool checked)
{
    ((MainWindow*)parent())->setFileFlag(checked);
}
