#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    {
        m_server=new TcpServerConnect(this);
        m_client=new TcpClientConnect(this);
    }

Widget::~Widget()
    {

    }
