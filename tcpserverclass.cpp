#include "tcpserverclass.h"

tcpServerClass::tcpServerClass(QObject *parent) : QObject(parent)
{
    datasFromClient = "-";
    commandDataFromServer = "-";
    bConnected =false;
    countTimeRead = 0;
    countTimeWrite = 0;
}

void tcpServerClass::startServer(int _port)
{
    myServer = new QTcpServer(this);

    connect(myServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));

    if(!myServer->listen((QHostAddress::Any),_port)){
        _str_server_status = "Fail";
    }else{
        _str_server_status = "Start OK";
        clientCount = 0;
    }
}

void tcpServerClass::WriteData(QByteArray _str)
{
    countTimeRead = 0;
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    datasFromClient = sender->readAll();
    while(sender->waitForReadyRead(50))
        datasFromClient.append(sender->readAll());
}

void tcpServerClass::onNewConnection()
{
    myClient = myServer->nextPendingConnection();

    connect(myClient, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(myClient, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
    _str_server_status = "Start OK";
    clientCount = 1;
    bConnected = true;
}

void tcpServerClass::onReadyRead()
{
    countTimeRead = 0;
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    datasFromClient = sender->readAll();
    while(sender->waitForReadyRead(50))
        datasFromClient.append(sender->readAll());
}

void tcpServerClass::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    _str_server_status = "-";

    if(socketState == QAbstractSocket::UnconnectedState)
    {
       myClient->close();
       myClient->deleteLater(); //moudtom
       _str_server_status = "Start OK";
       clientCount = 0;
       bConnected = false;
    }
}
