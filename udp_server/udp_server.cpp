#include "udp_server.h"

#include <QtNetwork/QUdpSocket>

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

