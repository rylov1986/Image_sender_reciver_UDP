#include "udp_server.h"

#include <QCoreApplication>
#include <QtNetwork/QUdpSocket>
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

    //loadPicture(QString("picture5.bmp"));


   // w.setTitle(QString("GGGGGG"));
  //  w.show();



    //test byte array integers
    QByteArray data("*********************");
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);

    stream << quint32(12)
           << quint32(13)
           << quint32(14);

    data.append(arr);
    qDebug() << "test byte array integers";

    qDebug() << data;

    quint32 baster =  111;
    data.append(reinterpret_cast<const char *>(&baster), sizeof(quint32));
    data.append(reinterpret_cast<const char *>(&baster), sizeof(quint32));
    data.append(reinterpret_cast<const char *>(&baster), sizeof(quint32));

    qDebug() << data;

    quint32 id, datagrams_count, image_size;
    QDataStream stream_read(&arr, QIODevice::ReadOnly);
    stream_read >> id
           >> datagrams_count
           >> image_size;

     qDebug() << "id=" << id << " datagrams_count=" << datagrams_count << " image_size="  << image_size ;

     data.resize(data.size() - sizeof(quint32)*6);

     qDebug() << data;

    //!test byte array integers
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

   // receiver_->writeDatagram(Data, QHostAddress::LocalHost, 2021);
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
     quint32 pos = 0;

     quint32 arrsize = base64Image.size();
     QList<QByteArray> arrays;
     quint32 id = 0;
     while(pos<arrsize){ //arrsize
         QByteArray arr = base64Image.mid(pos, 1024);

         quint32 part_size = arr.size();
         arr.append(reinterpret_cast<const char *>(&pos), sizeof(quint32));
         arr.append(reinterpret_cast<const char *>(&part_size), sizeof(quint32));
         arr.append(reinterpret_cast<const char *>(&arrsize), sizeof(quint32));

         arrays << arr;
         pos+=1024;//arr.size();

         // just for test
         quint32 pos, image_size;
         QDataStream stream_read(&arr, QIODevice::ReadOnly);
         stream_read >> pos
                >> part_size
                >> image_size;
         qDebug() << "pos="        << *reinterpret_cast<quint32*>(&arr[arr.size() - 3*sizeof(quint32)])
                 << " part_size="  << *reinterpret_cast<quint32*>(&arr[arr.size() - 2*sizeof(quint32)])
                 << " image_size=" << *reinterpret_cast<quint32*>(&arr[arr.size() -   sizeof(quint32)]) ;

         // just for test
     }

     for (auto& arr: arrays)
     {
         qDebug() << "arr.size() " <<arr.size();
         //qDebug() << arr;


    //     QJsonObject record;
    //     record.insert("id", QJsonValue{double(id)});
    //     record.insert("data", QJsonValue(QLatin1String(arr)));
   //      record.insert("image_size", QJsonValue{arrsize});//image_size
   //      record.insert("parts_number", QJsonValue(pos));
    //     QJsonDocument recordDoc(record);
   //      QByteArray Data;
  //       Data.append(recordDoc.toJson());

/*
         QByteArray datagram_info;
         QDataStream stream(&datagram_info, QIODevice::WriteOnly);

         stream << quint32(id)
                << quint32(arrays.size())
                << quint32(arrsize);

         arr.append(datagram_info);
         quint32 id, datagrams_count, image_size;
         QDataStream stream_read(&arr, QIODevice::ReadOnly);
         stream_read >> id
                >> datagrams_count
                >> image_size;

          qDebug() << "id=" << id << " datagrams_count=" << datagrams_count << " image_size="  << image_size ;
*/
         receiver_->writeDatagram(arr, QHostAddress::LocalHost, 2021);

         qDebug() << "id="<< id;

         id++;

         delay(5);
     }

     //test deserialization
     QByteArray raw_image;
     for( auto& i : arrays)
     {
          raw_image.append(i);
     }
     qDebug() << "raw image size = " << raw_image.size();
     //qDebug() << "raw image: " << raw_image;

     img->loadFromData(QByteArray::fromBase64(raw_image), "BMP");
     img->save("deserialized.bmp", "BMP");


     delete img;
}


MainWindow::MainWindow(QWidget *parent )
    : QMainWindow(parent)
{

}



quint32 byteArrayToUint32(const QByteArray &bytes)
{
    auto count = bytes.size();
    if (count == 0 || count > 4) {
        return 0;
    }
    quint32 number = 0U;
    for (int i = 0; i < count; ++i) {
        auto b = static_cast<quint32>(bytes[count - 1 - i]);
        number += static_cast<quint32>(b << (8 * i));
    }
    return number;
}



void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
