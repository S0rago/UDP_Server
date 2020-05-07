#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "myudpsocket.h"
//#include "keyfilter.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

protected slots:
    void clicked();
    void updateMessages(QString, QString);
    void updateUserList(QMap<QString, quint16>);

private:
    MyUDPSocket socket;
    QString message;
    Ui::Dialog *ui;
    bool eventFilter(QObject *, QEvent *);
};

#endif // DIALOG_H
