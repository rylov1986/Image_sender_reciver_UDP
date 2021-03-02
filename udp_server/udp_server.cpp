#include "udp_server.h"

#include <QCoreApplication>
#include <QBuffer>
#include <QDir>
#include <QtGui/QImage>
#include <QList>
#include <QByteArray>
#include <QTime>


UDPserver::UDPserver(QObject *parent):QObject(parent)
{
    receiver_ = new QUdpSocket(this);
    receiver_->bind(QHostAddress::LocalHost,2020);

    connect(receiver_,SIGNAL(readyRead()),this,SLOT(serverReceive()));
}

void UDPserver::serverReceive()
{

    qDebug()<<"Data receiving by server ...";

    QByteArray buffer;

    buffer.resize(receiver_->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    receiver_->readDatagram(buffer.data(), buffer.size(), &sender , &senderPort);

    QString QbuffStr(buffer);
    QJsonDocument buffResponse = QJsonDocument::fromJson(QbuffStr.toUtf8());

    QJsonObject jsonObj = buffResponse.object();

    if(jsonObj.find("start") != jsonObj.end())
        serverFeedBack();
}

void UDPserver::serverFeedBack()
{

    qDebug() << "start load picture...";

    loadPicture(QString("picture5.bmp"));
}

void UDPserver::loadPicture(const QString& fileName)
{

   QImage *img = new QImage(QDir::currentPath() + "/" + fileName);
   img->load(QDir::currentPath() + "/" + fileName, "BMP");

   QByteArray base64Image;

   if(!img->isNull())
   {
       QBuffer buffer;
       buffer.open(QIODevice::ReadWrite);
       img->save(&buffer, "bmp");
       //serialization
       base64Image = buffer.buffer().toBase64();
   }
   else
       qDebug() << "ERROR: cannot read Image " << fileName;


     //split serialized image, because datagram size must be less than 1500 bytes
     quint32 pos = 0;
     quint32 arrsize = base64Image.size();
     QList<QByteArray> arrays;
     quint32 id = 0;
     while(pos<arrsize){
         QByteArray arr = base64Image.mid(pos, 1024);
         quint32 part_size = arr.size();
         arr.append(reinterpret_cast<const char *>(&pos), sizeof(quint32));
         arr.append(reinterpret_cast<const char *>(&part_size), sizeof(quint32));
         arr.append(reinterpret_cast<const char *>(&arrsize), sizeof(quint32));

         arrays << arr;
         pos+=1024;

         // just for test
         //quint32 pos, image_size;
         //QDataStream stream_read(&arr, QIODevice::ReadOnly);
         //stream_read >> pos
         //       >> part_size
         //       >> image_size;
         //qDebug() << "pos="        << *reinterpret_cast<quint32*>(&arr[arr.size() - 3*sizeof(quint32)])
         //        << " part_size="  << *reinterpret_cast<quint32*>(&arr[arr.size() - 2*sizeof(quint32)])
         //        << " image_size=" << *reinterpret_cast<quint32*>(&arr[arr.size() -   sizeof(quint32)]) ;
         // !just for test
     }



     //send each part in UDP
     for (auto& arr: arrays)
     {
         qDebug()  << "id "<< id << " arr.size() = " << arr.size();
         //qDebug() << arr;

         receiver_->writeDatagram(arr, QHostAddress::LocalHost, 2021);

         id++;

         delay(5); //if send immediate - possible problem of quality on client side
     }

        //test deserialization on server side
        //     QByteArray raw_image;
        //     for( auto& i : arrays)
        //     {
        //          raw_image.append(i);
        //     }
        //     qDebug() << "raw image size = " << raw_image.size();
        //     //qDebug() << "raw image: " << raw_image;
        //     img->loadFromData(QByteArray::fromBase64(raw_image), "BMP");
        //     img->save("deserialized.bmp", "BMP");


     delete img;
}


MainWindow::MainWindow(QWidget *parent )
    : QMainWindow(parent)
{

}


void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
