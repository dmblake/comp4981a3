#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    server = (char*)malloc(25);
    port = (char*)malloc(25);
    username = (char*)malloc(25);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sendButton_clicked()
{
    qDebug() << "Clicked";

    QString msg =  ui->message_edit->toPlainText();
    QByteArray msgArray = msg.toUtf8();
    send_msg(sockfd,msgArray.data(),username,BUFLEN,0);
}

void MainWindow::on_actionConnect_triggered()
{
    qDebug() << "Connect";
    ConnectDialog c(this);
    c.setModal(true);
    c.exec();
    sockfd = connect_to_server(server,atoi(port));

    pthread_create(&thrd1,0,recvMsg, NULL);

    if(sockfd == -1){
        qDebug () <<  "unable to connect";
    }



}


void* MainWindow::recvMsg(void* param){
    int sock = *(int*) param;
    int bytesRead = BUFLEN;
    char * bp;
    char buf[BUFLEN];

    int n = 0;
    bp = buf;
    while ((n = recv (sock, buf, bytesRead, 0)) < BUFLEN)
   {
       bp += n;
       bytesRead-= n;
   }

    QString text = QString(bp);
    ui->recvBox->append(text);
}

void MainWindow::on_actionDisconnect_triggered()
{
    qDebug() << "Disconnect";
}

void MainWindow::setUserName(char* name){
    strcpy(username,name);
}

void MainWindow::setServer(char* serv){
  strcpy(server, serv);
}

void MainWindow::setPort(char* por){
    strcpy(port,por);
}


