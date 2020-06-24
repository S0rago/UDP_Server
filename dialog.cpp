#include<QKeyEvent>
#include "dialog.h"
#include "ui_dialog.h"
#include "myudpsocket.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);
    ui->msgBox->installEventFilter(this);
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(clicked()));
    connect(&socket, SIGNAL(doneReading(QString, QString)), this, SLOT(updateMessages(QString, QString)));
    connect(&socket, SIGNAL(newUser(QMap<QString, quint16>)), this, SLOT(updateUserList(QMap<QString, quint16>)));
}
Dialog::~Dialog() {
    delete ui;
}

void Dialog::clicked() {
    QString str, msg;
    str.append(ui->msgBox->toPlainText());
    if (str.length() > 0) {
        qDebug() << "<You>: " << str;
        msg.append(QString("<%1> %2").arg("this", str));
        ui->logBox->append(msg);
        ui->msgBox->clear();
        socket.SendMessage(str);
    }
}

void Dialog::updateMessages(QString sender, QString message) {
    ui->logBox->append(QString("<%1> %2").arg(sender, message));
}

void Dialog::updateUserList(QMap<QString, quint16> userList) {
    ui->userListBox->clear();

    QMap<QString, quint16>::iterator iterMap = userList.begin();
    while(iterMap != userList.end()) {
        ui->userListBox->append(iterMap.key());
        iterMap++;
    }
}

bool Dialog::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == ui->msgBox && event->type()==QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ((key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return)) {
            clicked();
        } else {
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}
