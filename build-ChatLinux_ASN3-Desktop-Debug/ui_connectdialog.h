/********************************************************************************
** Form generated from reading UI file 'connectdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QTextEdit *ipAddress;
    QTextEdit *port;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QLabel *label_3;
    QTextEdit *usernames;
    QCheckBox *fileCheck;

    void setupUi(QDialog *ConnectDialog)
    {
        if (ConnectDialog->objectName().isEmpty())
            ConnectDialog->setObjectName(QStringLiteral("ConnectDialog"));
        ConnectDialog->resize(361, 225);
        ipAddress = new QTextEdit(ConnectDialog);
        ipAddress->setObjectName(QStringLiteral("ipAddress"));
        ipAddress->setGeometry(QRect(90, 70, 241, 31));
        ipAddress->setTabChangesFocus(true);
        port = new QTextEdit(ConnectDialog);
        port->setObjectName(QStringLiteral("port"));
        port->setGeometry(QRect(90, 120, 241, 31));
        port->setTabChangesFocus(true);
        label = new QLabel(ConnectDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 80, 81, 20));
        label_2 = new QLabel(ConnectDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 130, 67, 17));
        pushButton = new QPushButton(ConnectDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(230, 190, 99, 27));
        label_3 = new QLabel(ConnectDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 30, 71, 17));
        usernames = new QTextEdit(ConnectDialog);
        usernames->setObjectName(QStringLiteral("usernames"));
        usernames->setGeometry(QRect(90, 20, 241, 31));
        usernames->setTabChangesFocus(true);
        fileCheck = new QCheckBox(ConnectDialog);
        fileCheck->setObjectName(QStringLiteral("fileCheck"));
        fileCheck->setGeometry(QRect(90, 160, 97, 22));
        QWidget::setTabOrder(usernames, ipAddress);
        QWidget::setTabOrder(ipAddress, port);
        QWidget::setTabOrder(port, fileCheck);
        QWidget::setTabOrder(fileCheck, pushButton);

        retranslateUi(ConnectDialog);

        QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
        ConnectDialog->setWindowTitle(QApplication::translate("ConnectDialog", "Dialog", 0));
        label->setText(QApplication::translate("ConnectDialog", "IP Address", 0));
        label_2->setText(QApplication::translate("ConnectDialog", "Port", 0));
        pushButton->setText(QApplication::translate("ConnectDialog", "Connect", 0));
        label_3->setText(QApplication::translate("ConnectDialog", "Username", 0));
        usernames->setPlaceholderText(QString());
        fileCheck->setText(QApplication::translate("ConnectDialog", "File Dump", 0));
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDIALOG_H
