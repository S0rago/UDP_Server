#include "myudpsocket.h"
#include <QTextStream>
#include <QDataStream>
#include <QRegExp>

MyUDPSocket::MyUDPSocket(QObject *parent) :
    QObject(parent) {
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::AnyIPv4, getBindPort());
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug("server started");
}

void MyUDPSocket::SendMessage(QString input, QString recvFromAddr) {
    //message+spearator byte+crc number
    QByteArray response;
    response.append(recvFromAddr != NULL ? recvFromAddr : socket->localAddress().toString()).append(0xFF);
    response.append(input.toUtf8());
    unsigned int crc = CRC32(response.data(), response.length());
    response.append(0xFF);
    response.append(QByteArray::number(crc));

    QMap<QString, quint16>::iterator iterMap = userList.begin();
    while(iterMap != userList.end()) {
        if(iterMap.key() != recvFromAddr) {
            socket->writeDatagram(response, QHostAddress(iterMap.key()), iterMap.value());
        }
        iterMap++;
    }
}

void MyUDPSocket::readyRead() {
    while(socket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
        QByteArrayList list = ParseMessage(buffer);
        QByteArray message = list.at(0);
        unsigned int chksum = list.at(1).toUInt();

        if (chksum == CRC32(message.data(),message.length())) {
            QString senderStr = sender.toString();
            AddUser(senderStr, senderPort);
            emit doneReading(senderStr, message);
            QString toPrint = QString("<%1>: %2").arg(senderStr, QString::fromUtf8(message.data(), message.size()));
            qDebug().noquote() << toPrint;
            SendMessage(message, senderStr);
        }
        else qDebug("Recieve error");

        if (QString::fromUtf8(message) == "end") {
            socket->close();
            exit(0);
            break;
        }
    }
}

unsigned int MyUDPSocket::CRC32(char *buf, unsigned long len) {
    unsigned long crc_table[256];
    unsigned long crc;
    for (int i = 0; i < 256; i++) {
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

QByteArrayList MyUDPSocket::ParseMessage(QByteArray arr) {
    //client addr 0xFF message 0xFF crc sum
    QByteArrayList list = arr.split(0xFF);
    return list;
}

quint16 MyUDPSocket::getBindPort() {
    QTextStream s(stdin);
    QString input;
    QRegExp re("(\\d{1,5})");
    for (int i = 5; i > 0; i--) {
        qDebug("Enter port to bind to: ");
        input = s.readLine();
        if (re.exactMatch(input)) {
            return input.toInt();
        }
        qDebug() << "You have " << i-1 << "tries left";
    }
    qDebug("Port will be set to 6000");
    return 6000;
}

void MyUDPSocket::AddUser(QString senderAdr, quint16 senderPort) {
    if (!userList.contains(senderAdr)) {
        userList.insert(senderAdr, senderPort);
        emit newUser(userList);
        qDebug() << "new user: " << senderAdr;
    } else if(userList.value(senderAdr) != senderPort) {
        userList.insert(senderAdr, senderPort);
        qDebug() << "new port for: " << senderAdr;
    }
}
