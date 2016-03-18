#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "network.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>
#include <netdb.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();

    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
