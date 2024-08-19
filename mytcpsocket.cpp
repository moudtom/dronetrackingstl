#include "mytcpsocket.h"

myTcpSocket::myTcpSocket(QObject *parent) : QObject(parent)
{
    sc = new QTcpSocket(this);
    connect(sc, SIGNAL(connected()),this, SLOT(connected()));
    connect(sc, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(sc, SIGNAL(readyRead()),this, SLOT(readData()));
    bIsSocketConnect = false;
}

void myTcpSocket::start(QString _ip, int _port)
{
    port = _port;
    qDebug() << "Start Connecting to port " << port;

    // this is not blocking call
    sc->connectToHost(_ip, _port);

    // we need to wait...
    if(!sc->waitForConnected(5000))
    {
        qDebug() << "port " << port << " CONNECTION Error: " << sc->errorString();
        bIsSocketConnect = false;
    }
}

void myTcpSocket::connected()
{
    qDebug()  << "port " << port << " is Connected...";
    _str_server_status = "Start OK";
    bIsSocketConnect = true;
}

void myTcpSocket::disconnected()
{
    qDebug()  << "port " << port << " is disconnected...";
    _str_server_status = "fail";
    bIsSocketConnect = false;
}

void myTcpSocket::readData()
{
    // read the data from the socket
    dataInputFromServer.clear();
    dataInputFromServer = sc->readAll();
    bNewDataFromServer = true;
}

void myTcpSocket::writeData(const char* _str,uint16_t sizedata)
{
    sc->write(_str,sizedata);
    commandDataFromServer = _str;
}


