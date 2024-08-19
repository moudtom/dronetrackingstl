#ifndef TCPSERVERCLASS_H
#define TCPSERVERCLASS_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class tcpServerClass : public QObject
{
    Q_OBJECT
public:
    explicit tcpServerClass(QObject *parent = nullptr);

    void startServer(int _port);
    void WriteData(QByteArray _str);
    QString getHostIP();

    QTcpServer *myServer;
    QTcpSocket *myClient;
    QList<QTcpSocket*> mySockets;
    QByteArray datasFromClient;
    QByteArray commandDataFromServer;
    QString _str_server_status;

    bool bConnected;
    int clientCount;

    unsigned int countTimeRead;
    unsigned int countTimeWrite;
public slots:
    void onNewConnection();
    void onReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
signals:

};

#endif // TCPSERVERCLASS_H
