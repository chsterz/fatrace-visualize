#include "mainwindow.h"
#include <QDebug>
#include <QTextStream>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <limits>

void MainWindow::parseFile(QFile &file)
{
    QTextStream in(&file);
    QString timeStampString, processString, modeString, fileString;
    int linecount = 0;

    m_firstTimeStamp = std::numeric_limits<quint64>::max();
    m_lastTimeStamp = std::numeric_limits<quint64>::min();

    while (!in.atEnd())
    {
        in >> timeStampString;

        QDateTime ts = QDateTime::fromString(timeStampString.mid(0,12), "HH:mm:ss.zzz");
        quint64 millis = ts.toMSecsSinceEpoch();
        quint64 micros = timeStampString.mid(12,3).toInt();
        micros = millis*1000 + micros;

        //get first and last ts
        m_firstTimeStamp = std::min(m_firstTimeStamp, micros);
        m_lastTimeStamp = std::max(m_lastTimeStamp, micros);

        in >> processString; // ignore that
        in >> modeString;
        in >> fileString;

        FileAction action;
        for(const QChar &c: modeString)
        {
            if (c == QChar('C'))
                action = FileAction::Close;
            if (c == QChar('R'))
                action = FileAction::Read;
            if (c == QChar('O'))
                action = FileAction::Open;
            if (c == QChar('W'))
                action = FileAction::Write;

            m_events.insert(fileString, FileEvent(action, micros));
        }
        linecount++;
    }
}

MainWindow::MainWindow(const QString &fileName, QWidget *parent = nullptr)
    : QMainWindow(parent)
{
    QFile fatraceFile(fileName);
    if (fatraceFile.open(QFile::ReadOnly))
        parseFile(fatraceFile);
    else
        qWarning() << "Could not open file" << fatraceFile.fileName();
}

MainWindow::~MainWindow()
{



}
