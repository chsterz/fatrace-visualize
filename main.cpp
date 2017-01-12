#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString fatraceFileName = QString(argv[1]);
    MainWindow w(fatraceFileName, nullptr);
    w.show();

    return a.exec();
}
