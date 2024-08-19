#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <vector>
#include <QDebug>

class myTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit myTcpSocket(QObject *parent = nullptr);
    void start(QString _ip, int _port);
    void writeData(const char* _str,uint16_t sizedata);
    bool bIsSocketConnect;
    bool bNewDataFromServer;
    QTcpSocket *sc;
    QByteArray dataInputFromServer;
    QString _str_server_status;
    QByteArray commandDataFromServer;
signals:

public slots:
    void connected();
    void disconnected();
    void readData();

private:
    int port;

};

#endif // MYTCPSOCKET_H
