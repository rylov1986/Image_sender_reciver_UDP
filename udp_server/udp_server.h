#pragma once

#include <iostream>
#include <QtNetwork/QUdpSocket>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>

class UDPserver : public QObject
{
    Q_OBJECT

private:

    QUdpSocket *receiver_;

public:

    UDPserver(QObject *parent = 0);

signals:
public slots:
    void serverReceive();
    void serverFeedBack();

};


