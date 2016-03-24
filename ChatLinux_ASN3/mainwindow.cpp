#include "mainwindow.h"

/*-------------------------------------------------------------------------------------------------
 * SOURCE FILE:     mainwindow.cpp - The UI that allows client to interact with the program
 *
 * PROGRAM:         ChatLinux_ASN3
 *
 * FUNCTIONS:       MainWindow::MainWindow(QWidget *parent)
 *                  MainWindow::~MainWindow()
 *                  void MainWindow::on_sendButton_clicked()
 *                  void MainWindow::on_actionConnect_triggered()
 *                  void* MainWindow::recvMsg(void* param)
 *                  void MainWindow::on_actionDisconnect_triggered()
 *                  void MainWindow::setUserName(char* name)
 *                  void MainWindow::setServer(char* serv)
 *                  void MainWindow::setPort(char* por)
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
 *                  in the connectdialog.cpp.
 *
 *------------------------------------------------------------------------------------------------*/
bool MainWindow::fileFlag;
char* MainWindow::username;
std::vector<std::string> MainWindow::clientsConnected;
/*-------------------------------------------------------------------------------------------------
 * FUNCTION         MainWindow
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       MainWindow(QWidget *parent) :QMainWindow(parent), ui(new UI::MainWindow)
 *
 * RETURNS:         void
 *
 * NOTES:           This is a constructor for the MainWindow class.
 *
 *------------------------------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    server = (char*)malloc(25);
    port = (char*)malloc(25);
    username = (char*)malloc(25);
    ui->setupUi(this);
    ui->message_edit->setDisabled(true);
    ui->message_edit->setPlaceholderText("Type Message Here");
    ui->actionDisconnect->setDisabled(true);
}


/*-------------------------------------------------------------------------------------------------
 * FUNCTION         ~MainWindow
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       ~MainWindow()
 *
 * RETURNS:         void
 *
 * NOTES:           This is a deconstructor for the MainWindow class.
 *
 *------------------------------------------------------------------------------------------------*/

MainWindow::~MainWindow()
{
    delete ui;
}


/*-------------------------------------------------------------------------------------------------
 * FUNCTION         on_sendButton_clicked
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void on_sendButton_clicked()
 *
 * RETURNS:         void
 *
 * NOTES:           This function allows functionality for a button that uses the send_msg
 *                  function from client.cpp to send messages to the server (which echoes
 *                  the message to all clients)
 *
 *------------------------------------------------------------------------------------------------*/
void MainWindow::on_sendButton_clicked()
{
    qDebug() << "Clicked";

    QString msg =  ui->message_edit->toPlainText();
    ui->recvBox->setTextColor(Qt::green);
    QString str(username);
    str.append(" : "+msg);
    ui->recvBox->append(str);
    QByteArray msgArray = msg.toUtf8();

    QString filename= username;
    filename.append(".txt");
    QFile file (filename);
    if(fileFlag == true && !(file.isOpen())){
        if ( file.open(QIODevice::Append) )
        {
            QTextStream stream( &file );
            stream <<str<< endl;
        }
    } else if (fileFlag == true){
        QTextStream stream( &file );
        stream <<str<< endl;
    }

    send_msg(sockfd,msgArray.data(),username,BUFLEN,0);    

    ui->message_edit->clear();
}


/*-------------------------------------------------------------------------------------------------
 * FUNCTION         on_actionConnect_triggered
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       March 23rd 2016 - added functionality to send username when connected
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void on_actionConnect_triggered()
 *
 * RETURNS:         void
 *
 * NOTES:           This function allows functionality for a menu item. It allows for a new
 *                  dialog to be opened to type the ip addresss and port (along with a username)
 *                  of the server to connect to. Calls connect_to_server function from client.cpp
 *                  then creates a thread to constantly recieve messages from server (other
 *                  clients) without interupting the ui thread.
 *------------------------------------------------------------------------------------------------*/

void MainWindow::on_actionConnect_triggered()
{
    ConnectDialog c(this);
    c.setModal(true);
    c.exec();

    if((sockfd = connect_to_server(server,atoi(port))) == -1){
        QMessageBox msgBox;
        msgBox.setText("Can't connect to server. Please try again.");
        msgBox.exec();
        return;
    }
    this->setWindowTitle(username);
    ui->message_edit->setEnabled(true);
    ui->actionConnect->setDisabled(true);
    ui->actionDisconnect->setEnabled(true);
    pthread_create(&thrd1,0,recvMsg, this);
    char msg [30] = {0};
    msg [0] = 4;
    strcat(msg+1,username);
    if ((send(sockfd, msg, BUFLEN, 0)) == 0)
    {
        perror("Send error");
        return;
    }
}

/*-------------------------------------------------------------------------------------------------
 * FUNCTION         recvMsg
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       March 23rd 2016 - added functionality to receive all users connected from
 *                                    server and append it to client ui
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void* recvMsg(void* param)
 *
 * RETURNS:         void
 *
 * NOTES:           This is a function for the receive message thread. Called when pthread is
 *                  created.
 *------------------------------------------------------------------------------------------------*/

void* MainWindow::recvMsg(void* param){

    int sock = ((MainWindow*)param)->sockfd;
    int bytesRead = BUFLEN;
    char * bp;
    char buf[BUFLEN];

    int n = 0;
    bp = buf;


    while (1)
    {
        while ((n = recv (sock, buf, bytesRead, 0)) < BUFLEN)
        {
            bp += n;
            bytesRead-= n;
        }

        QString filename= username;
        filename.append(".txt");
        QFile file (filename);

        if(buf[0] == 5){
            ((MainWindow*)param)->ui->viewClients->clear();
            std::string str;
            std::istringstream iss(buf+1);
            while(iss>>str){
                clientsConnected.push_back(str);
            }

            for(std::vector<std::string>::iterator it = clientsConnected.begin(); it != clientsConnected.end(); ++it) {
                QString qstr = QString::fromStdString(*it);
                ((MainWindow*)param)->ui->viewClients->addItem(qstr);
            }
            clientsConnected.clear();
        }
        else{
            QString text = QString(bp);
            ((MainWindow*)param)->ui->recvBox->setTextColor(Qt::black);
            ((MainWindow*)param)->ui->recvBox->append(text);

            if(fileFlag == true && !(file.isOpen())){
                if ( file.open(QIODevice::Append) )
                {
                    QTextStream stream( &file );
                    stream <<bp<< endl;
                }
            } else if (fileFlag == true){
                QTextStream stream( &file );
                stream <<bp<< endl;
            }
        }

    }
}

/*-------------------------------------------------------------------------------------------------
 * FUNCTION         on_actionDisconnect_triggered
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void on_actionDisconnect_triggered()
 *
 * RETURNS:         void
 *
 * NOTES:           This function allows functionality for a menu item.Calls disconnect_from_server
 *                  function from client.cpp
 *------------------------------------------------------------------------------------------------*/

void MainWindow::on_actionDisconnect_triggered()
{
    if((disconnectSocket(sockfd))== 0){
        QMessageBox msgBox;
        msgBox.setText("You're disconnected!");
        msgBox.exec();
        this->setWindowTitle("Client");
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setDisabled(true);
        ui->message_edit->setDisabled(true);
    }
    return;
}

/*-------------------------------------------------------------------------------------------------
 * FUNCTION         setUserName
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void setUserName(char* name)
 *
 * RETURNS:         void
 *
 * NOTES:           This function sets the username that all messages will be tied to for the
 *                  client.
 *------------------------------------------------------------------------------------------------*/

void MainWindow::setUserName(char* name){
    strcpy(username,name);
}

/*-------------------------------------------------------------------------------------------------
 * FUNCTION         setServer
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void setServer(char* serv)
 *
 * RETURNS:         void
 *
 * NOTES:           This function sets the server that will be used by the connect_to_sever
 *                  function to connect to the server.
 *------------------------------------------------------------------------------------------------*/

void MainWindow::setServer(char* serv){
  strcpy(server, serv);
}

/*-------------------------------------------------------------------------------------------------
 * FUNCTION         setPort
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void setPort(char* por)
 *
 * RETURNS:         void
 *
 * NOTES:           This function sets the port that will be used by the connect_to_sever
 *                  function to connect to the port.
 *------------------------------------------------------------------------------------------------*/

void MainWindow::setPort(char* por){
    strcpy(port,por);
}

/*-------------------------------------------------------------------------------------------------
 * FUNCTION         setFileFlag
 *
 * DATE:            March 22nd 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       void setFileFlag(bool checked)
 *
 * RETURNS:         void
 *
 * NOTES:           This function sets the fileFlag in MainWindow class that will allow a
 *                  file dump to happen.
 *------------------------------------------------------------------------------------------------*/

void MainWindow::setFileFlag(bool checked){
    fileFlag = checked;
}

