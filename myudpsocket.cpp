#include "myudpsocket.h"
#include <QTextStream>
#include <QRegExp>

MyUDPSocket::MyUDPSocket(QObject *parent) :
    QObject(parent) {
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::AnyIPv4, getBindPort());
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug("server started");
}

void MyUDPSocket::SendMessage(QString input) {
    QByteArray response;
    response.append(input);
    response.append(CRC32(response.data(), response.length()));

    QMap<QString, quint16>::iterator iterMap = userList.begin();
    while(iterMap != userList.end()) {
        socket->writeDatagram(response, QHostAddress(iterMap.key()), iterMap.value());
    }
}

void MyUDPSocket::readyRead() {
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize() + 10);

    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
    QByteArrayList list = ParseMessage(buffer);
    QByteArray message = list.at(1);
    unsigned int chksum = list.at(0).toInt();

    if (chksum == CRC32(message.data(),message.length())) {
        QString senderStr = sender.toString();
        if (!userList.contains(senderStr)) {
            userList.insert(senderStr, senderPort);
            emit newUser(userList);
            qDebug() << "new user: " << senderStr;
        }
        emit doneReading(senderStr, message);

        QString toPrint = QString("<%1>: %2").arg(senderStr, message);
        qDebug() << toPrint;
    }
    else qDebug("Recieve error");

    if (QString(buffer) == "end") {
        socket->close();
        exit(0);
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
    crc = crc ^ 0xFFFFFFFFUL;
    //return crc ^ 0xFFFFFFFFUL;
    return crc;
}

QByteArrayList MyUDPSocket::ParseMessage(QByteArray arr) {
    QByteArrayList list;
    list.append(arr.right(10)); // CRC32 checksum
    list.append(arr.left(arr.length() - 10));
    //list.append(arr.left(4)); TODO add packet header
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
