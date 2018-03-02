#ifndef TCPCLIENTCONNECT_H
#define TCPCLIENTCONNECT_H

#include <QObject>
#include<QTcpSocket>
#include<QBuffer>
#include<QImage>
#include<QHostAddress>
#include<sendstruct.h>
class sendStruct;
class TcpClientConnect : public QObject
{
    Q_OBJECT
public:
    explicit TcpClientConnect(QObject *parent = nullptr);
    QTcpSocket *m_tcpsocket;
    bool m_isGetPartData;
    int m_requestDataSize;
public slots:
    void handleSendOutData(const sendStruct&);
    void handleGetRecieveData();
    void handleSocketConnected();
};

#endif // TCPCLIENTCONNECT_H

