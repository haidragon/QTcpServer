#include "sendstruct.h"

sendStruct::sendStruct(int type,QString description,int imageWidth,int imageHeight,
                       int  imageChannels,QByteArray byteData) :Type(type),
                       Description(description),ImageWidth(imageWidth),ImageHeight(imageHeight),ImageChannels(imageChannels),ByteData(byteData)

    {

    }

