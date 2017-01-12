#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <QScrollArea>
#include <QFileDialog>

#include "renderarea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(const QString &filename, QWidget *parent);
    ~MainWindow();
    static const QHash<QString, QString> s_eventOrderDict;

    void parseFile(QFile &file);

public slots:
    void openNewFile();


private:
    FileEventList m_events;
    quint64 m_firstTimeStamp;
    quint64 m_lastTimeStamp;

    QMenuBar *m_menuBar;
    QMenu *m_menu;
    QFileDialog *m_fileDialog;
    QScrollArea *m_scrollArea;
    RenderArea *m_renderArea;
};

#endif // MAINWINDOW_H
