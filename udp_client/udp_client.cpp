#include "udp_client.h"

JsonClient::JsonClient(QObject *parent) : QObject(parent)
{
    sender_ = new QUdpSocket(this);
    sender_->bind(QHostAddress::LocalHost, 2021);
    clientSend();
    connect(sender_,SIGNAL(readyRead()),this,SLOT(clientReceive()));
}

void JsonClient::clientSend()
{
    while(true)
    {
        QByteArray Data;
        QJsonObject localRecord;

        localRecord.insert("start", QJsonValue{});
        QJsonDocument doc(localRecord);

        qDebug("Client is going to send this JSON to server ...");
        qDebug(doc.toJson());

        Data.append(doc.toJson());
        sender_->writeDatagram(Data, QHostAddress::LocalHost, 2020);

        clientReceive();
    }
}

void JsonClient::clientReceive()
{
    qDebug() << "recive feedback";
}
