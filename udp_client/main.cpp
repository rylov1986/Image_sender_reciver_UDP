#include <QApplication>
#include <QObject>
#include <QDir>

#include "udp_client.h"

using namespace udpClient;

int main(int argc, char *argv[])
{
    QApplication clientApp(argc, argv);

    //  test how QOpenGLWidget works
    //QImage *img = new QImage(QDir::currentPath() + "/" + "picture5.bmp");
    //PictureRender w(*img);
    //w.show();


    UDPclient client;

    while( !client.alreadyRecivedImage()  )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }


    PictureRender w( client.get_image() );
    w.show();


    return clientApp.exec();
}
