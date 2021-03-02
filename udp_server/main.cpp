#include <QApplication>

#include "udp_server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UDPserver server;

    QMainWindow w;
    w.setWindowTitle("GGG");
    w.show();
    return a.exec();
}
