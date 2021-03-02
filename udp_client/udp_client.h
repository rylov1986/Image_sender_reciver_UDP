#pragma once

#include "opengl_widget.h"

#include <QtNetwork/QUdpSocket>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>

namespace udpClient {

class UDPclient : public QObject
{
    Q_OBJECT

private:
    QUdpSocket *sender_;
    QByteArray raw_image;
    quint32 count;
    bool image_recived;
    QImage completed_image;

public:
    UDPclient(QObject *parent = 0);
    bool alreadyRecivedImage();
    QImage& get_image();

signals:

public slots:
    void clientSend();
    void clientReceive();
};

} // namespace udpClient
