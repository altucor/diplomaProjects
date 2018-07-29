#include "mainwindow.h"
#include <QApplication>
#include <QThread>

#include "udpserverwrapper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QThread loadFromFileThread;

    UdpServerWrapper Server;
    Server.init();

    w.connectServer(Server);

    return a.exec();
}
