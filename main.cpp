#include <QApplication>
#include "myudpsocket.h"
#include "dialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Dialog window;
    window.show();
    return a.exec();
}
