#include "myudpsocket.h"

MyUDPSocket::MyUDPSocket(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::AnyIPv4, 6000);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug("server started");
}

void MyUDPSocket::SendMessage(QString string)
{
    QByteArray Data;
    Data.append(string);

    socket->writeDatagram(Data, QHostAddress::LocalHost, 6000);
}

void MyUDPSocket::readyRead()
{
    QByteArray buffer;

    buffer.resize(socket->pendingDatagramSize() + 10);

    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
    QByteArrayList list = ParseMessage(buffer);

    QByteArray message = list.at(1);
    unsigned int chksum = list.at(0).toInt();

    if (chksum != CRC32(message.data(),message.length()))
    {
        qDebug() << "Message from: " << sender.toString();
        qDebug() << "Message port: " << senderPort;
        qDebug() << "Message: " << message;
    }
    else qDebug("Recieve error");

    QByteArray response;
    response.append(message.data())
            .append(CRC32(response.data(), response.length()));

    socket->writeDatagram(response, sender, senderPort);
    if (QString(buffer) == "end") {
        socket->close();
        exit(0);
    }
}

unsigned int MyUDPSocket::CRC32(char *buf, unsigned long len)
{
    unsigned long crc_table[256];
    unsigned long crc;
    for (int i = 0; i < 256; i++)
    {
        crc = i;
        for (int j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
        crc_table[i] = crc;
    };
    crc = 0xFFFFFFFFUL;
    while (len--)
    crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFUL;
}

QByteArrayList MyUDPSocket::ParseMessage(QByteArray arr){
    QByteArrayList list;
    list.append(arr.right(10)); // CRC32 checksum
    list.append(arr.left(arr.length() - 10));
    //list.append(arr.left(4)); TODO add packet header
    return list;
}
