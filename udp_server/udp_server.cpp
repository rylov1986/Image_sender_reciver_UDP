#include "udp_server.h"

#include <QtNetwork/QUdpSocket>
#include <QBuffer>
#include <QDir>
#include <QtGui/QImage>
#include <QPixmap>
#include <QByteArray>


UDPserver::UDPserver(QObject *parent):QObject(parent)
{
    receiver_ = new QUdpSocket(this);
    receiver_->bind(QHostAddress::LocalHost,2020);

    connect(receiver_,SIGNAL(readyRead()),this,SLOT(serverReceive()));

    loadPicture(QString("picture5.bmp"));
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

    if(!jsonObj.find("start")->isNull())
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
}

void UDPserver::loadPicture(const QString& fileName)
{

   QImage *img = new QImage(QDir::currentPath() + "/" + fileName);

   QByteArray base64Image;
     if(!img->isNull())
     {
         QBuffer buffer;
         buffer.open(QIODevice::ReadWrite);
         img->save(&buffer, "BMP");

         //serialization
         base64Image = buffer.buffer().toBase64();
     }
     else
         qDebug() << "ERROR: cannot read Image " << fileName;


     //test deserialization
     img->loadFromData(QByteArray::fromBase64(base64Image), "BMP");
     img->save("deserialized.bmp", "BMP");


     delete img;
}

