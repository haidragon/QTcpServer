#ifndef TCPSERVERCONNECT_H
#define TCPSERVERCONNECT_H

#include <QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include<sendstruct.h>
#include<QImage>
#include<QBuffer>
class sendStruct;
class TcpServerConnect : public QObject
{
    Q_OBJECT
public:
    explicit TcpServerConnect(QObject *parent = nullptr);
private:
    QTcpServer *m_server;
    QTcpSocket *m_tcpsocket;
    bool m_isGetPartData;
    int m_requestDataSize;
public slots:
    void handleSendOutData(const sendStruct&);
    void handleGetRecieveData();
    void handleNewConnection();
};

#endif // TCPSERVERCONNECT_H
