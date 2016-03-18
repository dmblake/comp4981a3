#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "network.h"
#include "client.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

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

private slots:
    void on_sendButton_clicked();
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();


private:
    void* recvMsg(void* param);
    Ui::MainWindow *ui;    
    char* username;
    char* server;
    char* port;
    int sockfd;
    pthread_t* thrd1;
};

#endif // MAINWINDOW_H
