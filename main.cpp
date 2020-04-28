#include <QApplication>
#include "myudpsocket.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog window;
  //  MyUDPSocket client;
    window.show();

  //  QString message = "";

   // client.SendMessage(message);

//    QTextStream stream(stdin);
//    while(message.compare("end") != 0)
//    {
//        qInfo() << "enter message: ";
//        message = stream.readLine();
//        client.SendMessage(message);
//    }

    return a.exec();
}
