#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "renderarea.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(const QString &fileName, QWidget *parent);
    ~MainWindow();

    void parseFile(QFile &file);

    QMultiMap<QString, FileEvent> m_events;
    quint64 m_firstTimeStamp;
    quint64 m_lastTimeStamp;

    RenderArea *m_renderArea;
};

#endif // MAINWINDOW_H
