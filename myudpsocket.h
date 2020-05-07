#ifndef MYUDPSOCKET_H
#define MYUDPSOCKET_H

#include <QObject>
#include <QUdpSocket>

class MyUDPSocket : public QObject {
    Q_OBJECT
public:
    explicit MyUDPSocket(QObject *parent = 0);
    void SendMessage(QString input = "Hello from UDP!");
    unsigned int CRC32(char *buf, unsigned long len);
    QByteArrayList ParseMessage(QByteArray arr);

public slots:
    void readyRead();
signals:
    void doneReading(QString, QString);
    void newUser(QMap<QString, quint16>);
private:
    quint16 getBindPort();
    QUdpSocket *socket;
    QMap<QString, quint16> userList;
};

#endif // MYUDPSOCKET_H
