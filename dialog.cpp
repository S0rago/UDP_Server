#include "dialog.h"
#include "ui_dialog.h"
#include "myudpsocket.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clicked()));
    //connect(socket, SIGNAL(MyUDPSocket::readyRead()), socket, SLOT(MyUDPSocket::readyRead()));
}
Dialog::~Dialog()
{
    delete ui;
}

void Dialog::clicked()
{
    QString str, msg;
    str.append(ui->msgBox->toPlainText());
    if (str.length() > 0)
    {
        qDebug() << "Send pressed: " << str;
        msg.append(QString("<%1> %2").arg("this", str));
        ui->logBox->append(msg);
        ui->msgBox->clear();
    }
}
