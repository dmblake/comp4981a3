#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include "string.h"
#include "network.h"
#include "client.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include <QtDebug>
#include <vector>
#include <QFile>
#include <QMessageBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setUserName(char* name);
    void setServer(char* server);
    void setPort(char* port);
    void setFileFlag(bool checked);

private slots:
    void on_sendButton_clicked();
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();


private:
    static void* recvMsg(void* param);
    Ui::MainWindow *ui;    
    static char* username;
    char* server;
    char* port;
    int sockfd;
    pthread_t thrd1;
    static bool fileFlag;
    static std::vector<std::string> clientsConnected;
};

#endif // MAINWINDOW_H
