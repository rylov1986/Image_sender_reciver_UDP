#include "udp_server.h"

#include <QtNetwork/QUdpSocket>
#include <QBuffer>
#include <QDir>
#include <QtGui/QImage>
#include <QList>
#include <QByteArray>


UDPserver::UDPserver(QObject *parent):QObject(parent)
{
    receiver_ = new QUdpSocket(this);
    receiver_->bind(QHostAddress::LocalHost,2020);

    connect(receiver_,SIGNAL(readyRead()),this,SLOT(serverReceive()));

    //loadPicture(QString("picture5.bmp"));


   // w.setTitle(QString("GGGGGG"));
  //  w.show();
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
    QJsonObject record;
    record.insert("success", QJsonValue{});
    QJsonDocument recordDoc(record);
    QByteArray Data;
    Data.append(recordDoc.toJson());

    receiver_->writeDatagram(Data, QHostAddress::LocalHost, 2021);
    loadPicture(QString("picture5.bmp"));
}

void UDPserver::loadPicture(const QString& fileName)
{

   QImage *img = new QImage(QDir::currentPath() + "/" + fileName);
   img->load(QDir::currentPath() + "/" + fileName, "BMP");
   int image_size = img->sizeInBytes();

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
     int pos = 0;

     int arrsize = base64Image.size();
     //QList<QByteArray> arrays;
     volatile int id = 0;
     while(pos<image_size){ //arrsize
         QByteArray arr = base64Image.mid(pos, 1024);
         //arrays << arr;

         qDebug() << "arr.size() " <<arr.size();
         qDebug() << arr;


         QJsonObject record;
         record.insert("id", QJsonValue{double(id)});
         record.insert("data", QJsonValue(QLatin1String(arr)));
         record.insert("image_size", QJsonValue{arrsize});//image_size
         record.insert("parts_number", QJsonValue(pos));
         QJsonDocument recordDoc(record);
         QByteArray Data;
         Data.append(recordDoc.toJson());
         receiver_->writeDatagram(Data, QHostAddress::LocalHost, 2021);

         pos+=arr.size();
         id++;
     }

     //test deserialization
     //img->loadFromData(QByteArray::fromBase64(base64Image), "BMP");
     //img->save("deserialized.bmp", "BMP");


     delete img;
}


MainWindow::MainWindow(QWidget *parent )
    : QMainWindow(parent)
{

}
