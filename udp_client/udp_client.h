#pragma once

#include <QtNetwork/QUdpSocket>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>

using namespace std;

class JsonClient : public QObject
{
    Q_OBJECT

private:
    QUdpSocket *sender_;
    QByteArray raw_image;
    quint32 count;

public:
    JsonClient(QObject *parent = 0);

public slots:
    void clientSend();
    void clientReceive();
};

