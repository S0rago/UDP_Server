#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "myudpsocket.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    MyUDPSocket socket;
    QString message;
    Ui::Dialog *ui;
public slots:
    void clicked();
};

#endif // DIALOG_H
