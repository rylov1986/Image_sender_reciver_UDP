#include <QCoreApplication>

#include "udp_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UDPserver server;

    return a.exec();
}
