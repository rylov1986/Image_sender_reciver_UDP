#include <QCoreApplication>

#include "json_client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    JsonClient client;
    return a.exec();
}
