#include "udp_client.h"

#include <QImage>
#include <QBuffer>
#include <QDir>

JsonClient::JsonClient(QObject *parent)
    : QObject(parent),
      count(0)
{
    sender_ = new QUdpSocket(this);
    sender_->bind(QHostAddress::LocalHost, 2021);

    connect(sender_,SIGNAL(readyRead()),this,SLOT(clientReceive()));

    clientSend();
}

void JsonClient::clientSend()
{
    QByteArray Data;
    QJsonObject localRecord;

    localRecord.insert("start", QJsonValue{});
    QJsonDocument doc(localRecord);

    qDebug("Client is going to send this JSON to server ...");
    qDebug(doc.toJson());

    Data.append(doc.toJson());
    sender_->writeDatagram(Data, QHostAddress::LocalHost, 2020);
}

void JsonClient::clientReceive()
{
    qDebug() << "recive feedback from server";

    QByteArray buffer;

    buffer.resize(sender_->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    sender_->readDatagram(buffer.data(), buffer.size(), &sender , &senderPort);

    quint32 pos = *reinterpret_cast<quint32*>(&buffer[buffer.size() - 3*sizeof(quint32)]);
    quint32 datagram_size = *reinterpret_cast<quint32*>(&buffer[buffer.size() - 2*sizeof(quint32)]);
    quint32 image_size = *reinterpret_cast<quint32*>(&buffer[buffer.size() - sizeof(quint32)]);

    //raw_image.reserve(image_size);
    raw_image.replace(pos, datagram_size, buffer.data());

    //qDebug() << "pos=" << pos << " datagrams_count=" << datagram_size << " image_size="  << image_size << "count:  = " << count;
    //qDebug() << "raw size=" << raw_image.size() << " buffer size=" << buffer.size();

    count++;

    if( raw_image.size() == image_size )
    {
       qDebug() << "complete total datagrams accepted = " << count << " raw image size = " << raw_image.size();
       //qDebug() << "raw image: " << raw_image;

       QImage requested_image;

       requested_image.loadFromData(QByteArray::fromBase64(raw_image), "BMP");//QByteArray::fromBase64

       if(!requested_image.isNull())
           requested_image.save("CHECK_ME_client_recived.bmp", "BMP");

    }
}
