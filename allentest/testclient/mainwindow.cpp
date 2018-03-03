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
    this->socket=new QTcpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->socket->connectToHost("127.0.0.1",80100,QTcpSocket::ReadWrite);
        connect(this->socket,SIGNAL(connected()),this,SLOT(connected()));
}
void MainWindow:: connected(){
    QMessageBox::about(this,"提示","连接成功");
        connect(this->socket,SIGNAL(readyRead()),this,SLOT(readyread()));
}

void MainWindow:: readyread(){
    QMessageBox::about(this,"提示","准备读取");
    QByteArray arr=this->socket->readAll();
    QDataStream * dst=new QDataStream(&arr,QIODevice::ReadOnly);/******重点******/
    QString str1;
    QString str2;
    (*dst)>>str1>>str2;
    this->ui->textBrowser->setText(str1+str2);
    QMessageBox::about(this,"x",str1+str2);
}

void MainWindow::on_pushButton_2_clicked()
{
        QString str=this->ui->lineEdit->text();
        QByteArray arr;
        QDataStream dst(&arr,QIODevice::ReadWrite);/*QDataStream是继承于QIODevice的所以 必须要 设置读写权限*/
        dst<<QString("信息")<<str;/*这里一定要设置好数据类型*/
        this->socket->write(arr);
}
