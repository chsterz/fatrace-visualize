#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <QScrollArea>
#include <QFileDialog>
#include <QEvent>
#include <QWheelEvent>
#include <QDebug>

#include "renderarea.h"

class CtrlWheelFilter : public QObject
{
    Q_OBJECT

protected:
      bool eventFilter(QObject *obj, QEvent *event)
      {
          if (event->type() == QEvent::Wheel && static_cast<QWheelEvent *>(event)->modifiers() & Qt::ControlModifier)
          {
              event->ignore();
              return true;
          }
          return QObject::eventFilter(obj, event);
      }
};

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
    CtrlWheelFilter *m_wheelfilter;
};

#endif // MAINWINDOW_H
