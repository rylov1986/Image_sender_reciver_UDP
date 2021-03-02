#pragma once

#include <iostream>

#include <QMainWindow>
#include <QWidget>
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
    void loadPicture(const QString& fileName);
    QUdpSocket *receiver_;

public:

    UDPserver(QObject *parent = 0);

signals:
public slots:
    void serverReceive();
    void serverFeedBack();

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

};


void delay( int millisecondsToWait );

