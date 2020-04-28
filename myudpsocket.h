#ifndef MYUDPSOCKET_H
#define MYUDPSOCKET_H

#include <QObject>
#include <QUdpSocket>

class MyUDPSocket : public QObject
{
    Q_OBJECT
public:
    explicit MyUDPSocket(QObject *parent = 0);
    void SendMessage(QString string = "Hello from UDP!");
    unsigned int CRC32(char *buf, unsigned long len);
    QByteArrayList ParseMessage(QByteArray arr);


public slots:
    void readyRead();

private:
    QUdpSocket *socket;

};

#endif // MYUDPSOCKET_H
