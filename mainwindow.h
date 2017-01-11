#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <QScrollArea>

#include "renderarea.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(const QString &fileName, QWidget *parent);
    ~MainWindow();
    static const QHash<QString, QString> s_eventOrderDict;

    void parseFile(QFile &file);

    FileEventList m_events;
    quint64 m_firstTimeStamp;
    quint64 m_lastTimeStamp;

    QScrollArea *m_scrollArea;
    RenderArea *m_renderArea;
};

#endif // MAINWINDOW_H
