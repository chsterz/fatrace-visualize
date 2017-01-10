#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QHash>
#include <QString>
#include <QPair>
#include <QDateTime>

enum class FileAction{
    Close = 1,
    Read  = 2,
    Open  = 4,
    Write = 8
};

using micros = unsigned long;
using FileEvent = QPair<FileAction, quint64>;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:


    MainWindow(const QString &fileName, QWidget *parent);
    ~MainWindow();

    void parseFile(QFile &file);

    QMultiHash<QString, FileEvent> m_events;
    quint64 m_firstTimeStamp;
    quint64 m_lastTimeStamp;
};

#endif // MAINWINDOW_H
