#ifndef SENDSTRUCT_H
#define SENDSTRUCT_H

#include <QObject>
#include<QVariant>
#include<QDataStream>
struct valueStruct{
  int paulse;
  int speed;
  int acceleration;
  int code;
};
class sendStruct
{
public:
    explicit sendStruct(int Type,QString Description,int ImageWidth=-1,int ImageHeight=-1,
                        int  ImageChannels=-1,QByteArray ByteData=QByteArray(0));
    int  Type;
    QString Description;
    int  ImageWidth;
    int  ImageHeight;
    int  ImageChannels;
    int  FPS;
    QByteArray ByteData;
    sendStruct(){ Type=0; Description=""; ImageWidth=0; ImageHeight=0;
                  ImageChannels=0; ByteData=QByteArray(0);}
    int size()
        {
            int size=0;
            size=sizeof(int)*5+Description.size()*2+4+ByteData.size()+4;
            return size;
        }
    int size() const
        {
            int size=0;
            size=sizeof(int)*5+Description.size()*2+4+ByteData.size()+4;
            return size;
        }
    sendStruct &operator=(const sendStruct &other)
        {
            Type=other.Type;
            Description=other.Description;
            ImageWidth=other.ImageWidth;
            ImageHeight=other.ImageHeight;
            ImageChannels=other.ImageChannels;
            ByteData=QByteArray(other.ByteData);
            return *this;
        }
#ifndef QT_NO_DATASTREAM
    friend QDataStream& operator <<(QDataStream& out,const sendStruct& senstruct)
        {
            out.setVersion(QDataStream::Qt_4_7);
            out<<senstruct.Type
            <<senstruct.Description
            <<senstruct.ImageWidth
            <<senstruct.ImageHeight
            <<senstruct.ImageChannels
            <<senstruct.FPS
            <<senstruct.ByteData;
            return out;
        }
    friend QDataStream& operator >>(QDataStream& in,sendStruct& senstruct)
        {
            in.setVersion(QDataStream::Qt_4_7);
            in>>senstruct.Type
            >>senstruct.Description
            >>senstruct.ImageWidth
            >>senstruct.ImageHeight
            >>senstruct.ImageChannels
            >>senstruct.FPS
            >>senstruct.ByteData;
            return in;
        }
#endif
};
Q_DECLARE_METATYPE(sendStruct)
#endif // SENDSTRUCT_H
