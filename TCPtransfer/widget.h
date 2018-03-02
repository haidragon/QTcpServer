#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<tcpserverconnect.h>
#include<tcpclientconnect.h>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    TcpServerConnect *m_server;
    TcpClientConnect *m_client;
};

#endif // WIDGET_H
