#include "tcpclientconnect.h"

TcpClientConnect::TcpClientConnect(QObject *parent) : QObject(parent)
    {
        m_tcpsocket=new QTcpSocket(this);
        m_isGetPartData=false;
        m_requestDataSize=0;
        connect(m_tcpsocket,&QTcpSocket::readyRead,this,&TcpClientConnect::handleGetRecieveData);
        connect(m_tcpsocket,&QTcpSocket::disconnected,this,&TcpClientConnect::handleSocketConnected);
        m_tcpsocket->connectToHost(QHostAddress("127.0.0.1"),6868);
        if(m_tcpsocket->waitForConnected(3000)==false){
                qDebug()<<"connect error:"<<m_tcpsocket->errorString();
            }
    }

void TcpClientConnect::handleSendOutData(const sendStruct &data)
    {
        if((!m_tcpsocket)||m_tcpsocket->state()!=QAbstractSocket::ConnectedState)
            return;
        QDataStream out(m_tcpsocket);
        out<<data.size()<<data;
        m_tcpsocket->flush();
        /*把需要发送的数据封装在结构体里面发送*/
    }

void TcpClientConnect::handleGetRecieveData()
    {
        if((!m_tcpsocket)||m_tcpsocket->state()!=QAbstractSocket::ConnectedState)
            return;
        if(m_isGetPartData==false){
                if(m_tcpsocket->bytesAvailable()<sizeof(int))//待接收数据大小
                    return;
                else
                    {
                        QDataStream in(m_tcpsocket);
                        in>>m_requestDataSize;
                        m_isGetPartData=true;
                    }
            }
        if(m_isGetPartData==true){
                if(m_tcpsocket->bytesAvailable()<m_requestDataSize)
                    return;
                else
                    {
                        QDataStream in(m_tcpsocket);
                        sendStruct receiveData;
                        in>>receiveData;
                        m_requestDataSize=0;
                        m_isGetPartData=false;
                        /*
                        数据接收成功，放置在receiveData中，可以做其他处理
                        doSomething(receiveData);
                        */
                        qDebug()<<"receiveData type"<<receiveData.Type;
                        qDebug()<<"receiveData Description"<<receiveData.Description;
                        qDebug()<<"receiveData ByteData"<<receiveData.ByteData;
                        if(m_tcpsocket->bytesAvailable())
                            handleGetRecieveData();
                    }
            }
    }

void TcpClientConnect::handleSocketConnected()
    {
        sendStruct sendImageData;
        sendImageData.Type=0;
        sendImageData.Description=QString("this is image");
        QImage image(QSize(640,480),QImage::Format_RGB888);
        image.fill(Qt::gray);
        QBuffer buffur(&sendImageData.ByteData);
        buffur.open(QIODevice::ReadWrite);
        image.save(&buffur,"JPG");
        handleSendOutData(sendImageData);

        sendStruct sendPointData;
        sendPointData.Type=1;
        sendPointData.Description="this is point";
        QDataStream pointStream(&sendPointData.ByteData,QIODevice::WriteOnly);
        pointStream<<QPoint(100,100);
        handleSendOutData(sendPointData);

    }
