#include "mainwindow.h"
#include <QApplication>

/*-------------------------------------------------------------------------------------------
 * SOURCE FILE:     main.cpp -      An application that allows clients to connect to a server
 *                                  and talk to each other from different computers.
 *
 * PROGRAM:         ChatLinux_ASN3
 *
 * FUNCTIONS:       int main(int argc , char *argv[])
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * NOTES:          main.cpp is only for making the mainwindow that will run the gui for
 *                 the application
 *
 *------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------
 * FUNCTION         main
 *
 * DATE:            March 17th 2016
 *
 * REVISIONS:       N/A
 *
 * DESIGNER:        Dhivya Manohar
 *
 * PROGRAMMER:      Dhivya Manohar
 *
 * INTERFACE:       int main(int argc, char *argv[])
 *
 * RETURNS:         Returns a value that indicates how the program exited
 *
 * NOTES:           main.cpp is only for making the mainwindow that will run the gui for
 *                  the application
 *
 *------------------------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
