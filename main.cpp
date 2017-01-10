#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString fatraceFile = QString(argv[1]);
    MainWindow w(fatraceFile, nullptr);
    w.show();

    return a.exec();
}
