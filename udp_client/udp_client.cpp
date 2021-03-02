#include "udp_client.h"

#include <QImage>

JsonClient::JsonClient(QObject *parent)
    : QObject(parent),
      count(0)
{
    sender_ = new QUdpSocket(this);
    sender_->bind(QHostAddress::LocalHost, 2021);
    clientSend();
    connect(sender_,SIGNAL(readyRead()),this,SLOT(clientReceive()));
}

void JsonClient::clientSend()
{
    //while(true)
    {
        QByteArray Data;
        QJsonObject localRecord;

        localRecord.insert("start", QJsonValue{});
        QJsonDocument doc(localRecord);

        qDebug("Client is going to send this JSON to server ...");
        qDebug(doc.toJson());

        Data.append(doc.toJson());
        sender_->writeDatagram(Data, QHostAddress::LocalHost, 2020);

       // clientReceive();
    }
}

void JsonClient::clientReceive()
{
    qDebug() << "recive feedback from server";

    QByteArray buffer;

    buffer.resize(sender_->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    sender_->readDatagram(buffer.data(), buffer.size(), &sender , &senderPort);

    QString QbuffStr(buffer);
    QJsonDocument buffResponse = QJsonDocument::fromJson(QbuffStr.toUtf8());

    QJsonObject jsonObj = buffResponse.object();

    uint64_t image_size = jsonObj.value("image_size").toInteger();
    uint64_t pos = jsonObj.value("parts_number").toInt();
    uint32_t data_size = jsonObj.value("data").toString().size();

    raw_image.reserve(image_size);
 //   if( (count - 1)*1024 + data_size <= image_size )
    {

       qDebug() << "count: " << count;
       qDebug() << "datagram id = " << jsonObj.value("id").toInt();
       qDebug() << "data size: " << jsonObj.value("data").toString().size();
       qDebug() << "image_size: " << jsonObj.value("image_size").toInt();
       qDebug() << "parts_number: " << jsonObj.value("parts_number").toInt();

      // uint32_t id = jsonObj.value("id").toDouble();
       QByteArray serizlized_part = jsonObj.value("data").toString().toLatin1();
      // QByteArray part = QByteArray::fromBase64(serizlized_part);

       raw_image.replace(pos, data_size, serizlized_part );
       count+=data_size;
    }

    if( pos + data_size == image_size )
    {
      qDebug() << "count = " << count << "  image_size = " << image_size;
      QImage requested_image;
      requested_image.loadFromData(QByteArray::fromBase64(raw_image), "BMP");//QByteArray::fromBase64
      requested_image.save("recived.bmp", "BMP");
      count = 0;
    }
}
