#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sendButton_clicked()
{
    qDebug() << "Clicked";
    int num_sent;
    char * modified_message;
   // modified_message = add_username_to_msg(username, msg);
    if (modified_message == 0)
    {
        free(modified_message);
        return;
    }
    //if ((num_sent = send(sockfd, modified_message, buflen, flags)) == 0)
    {
        perror("Send error");
        free(modified_message);
        return;
    }

    free(modified_message);
    return;
}

void MainWindow::on_actionConnect_triggered()
{
    qDebug() << "Connect";
    ConnectDialog c;
    c.setModal(true);
    c.exec();
}



void MainWindow::on_actionDisconnect_triggered()
{
    qDebug() << "Disconnect";
}
