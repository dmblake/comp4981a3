#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

private slots:
    void on_pushButton_clicked();
    void on_fileCheck_clicked(bool checked);

private:
    Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_H
