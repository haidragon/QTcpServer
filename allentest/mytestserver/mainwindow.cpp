#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QDataStream>
#include<QString>
#include<QByteArray>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->socket=0;
    this->server=new QTcpServer(this);
    this->server->listen(QHostAddress::Any,80100);
    connect(this->server,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::newConnection(){
    this->socket=this->server->nextPendingConnection();
    QMessageBox::about(this,"提示","有新的连接！");
    connect(this->socket,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
}
void MainWindow::ReceiveData(){
   QMessageBox::about(this,"提示","正在读取");
   QByteArray arr=this->socket->readAll();
   QDataStream dst(arr);
   QString str1;
   QString str2;
   dst>>str1>>str2;
   this->ui->textBrowser->setText(str1+str2);
}
