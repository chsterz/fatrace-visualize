#include "mainwindow.h"
#include <QDebug>
#include <QTextStream>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <limits>
#include <QString>
#include <QMenuBar>
#include <QMenu>

void MainWindow::parseFile(QFile &file)
{
    QTextStream in(&file);
    QString timeStampString, processString, modeString, fileString;

    m_firstTimeStamp = std::numeric_limits<quint64>::max();
    m_lastTimeStamp = std::numeric_limits<quint64>::min();

    while (!in.atEnd())
    {
        in >> timeStampString;

        QDateTime ts = QDateTime::fromString(timeStampString.mid(0,12), "HH:mm:ss.zzz");
        quint64 millis = ts.toMSecsSinceEpoch();
        quint64 micros = timeStampString.mid(12,3).toInt();
        micros = millis*1000 + micros;

        if(!ts.isValid())
            continue;

        //get first and last ts
        m_firstTimeStamp = std::min(m_firstTimeStamp, micros);
        m_lastTimeStamp = std::max(m_lastTimeStamp, micros);

        in >> processString; // ignore that
        in >> modeString;
        in >> fileString;

        //fatrace has a crazy Order of Events, lets sanitize this if all are at the same timestamp
        if (modeString == "RCO")
            modeString = "ORC";
        if (modeString == "RO")
            modeString = "OR";
        if (modeString == "CR")
            modeString = "RC";

        //insert fused options separately ORC -> O, R, and C
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

            //look if filename already exists in Vector
            auto existingFile = std::find_if(m_events.begin(),
                                             m_events.end(),[&fileString](const auto &namedEventList){
                                                             return namedEventList.filename == fileString;});
            if (existingFile != m_events.cend())
            {
                // insert to existing
                existingFile->events.append(FileEvent(action, micros));
            }
            else
            {
                //insert new
                NamedEventList newFileEvents;
                QVector<FileEvent> newFileEventEntry;
                newFileEventEntry.append(FileEvent(action, micros));
                m_events.append(NamedEventList(fileString, newFileEventEntry));
            }
        }
    }
    //make relative to first Event
    for(auto &events: m_events)
        for(auto &event: events.events)
            event.second -= m_firstTimeStamp;

    for(auto &events: m_events)
        std::sort(events.events.begin(), events.events.end(), [](auto lhs, auto rhs) {
            if(lhs.second == rhs.second)
            {
                const int lhsMode = static_cast<int>(lhs.first);
                const int rhsMode = static_cast<int>(rhs.first);
                return lhsMode < rhsMode;
            }
            return lhs.second < rhs.second;
        });

    m_lastTimeStamp = m_lastTimeStamp - m_firstTimeStamp;
    m_firstTimeStamp = 0;
}

void MainWindow::openNewFile()
{
    QString openFilename = QFileDialog::getOpenFileName(this,"Select a fatrace file.");
    QFile fatraceFile(openFilename);
    if (fatraceFile.open(QFile::ReadOnly))
        parseFile(fatraceFile);
    else
        qWarning() << "Could not open file" << fatraceFile.fileName();
}


MainWindow::MainWindow(const QString &filename, QWidget *parent)
    : QMainWindow(parent)
{
    QString openFilename;
    if (filename.isEmpty())
        openFilename = QFileDialog::getOpenFileName(this,"Select a fatrace file.");
    else
        openFilename = filename;
    QFile fatraceFile(openFilename);
    if (fatraceFile.open(QFile::ReadOnly))
        parseFile(fatraceFile);
    else
        qWarning() << "Could not open file" << fatraceFile.fileName();

    setMinimumSize(QSize(1920, 1080));
    m_renderArea = new RenderArea(&m_events, m_lastTimeStamp);
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(m_renderArea);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->show();
    setCentralWidget(m_scrollArea);

    m_menuBar = new QMenuBar();
    m_menu = new QMenu("File");
    m_menu->addAction("Open…");
    connect(m_menu->actions().first(), SIGNAL(triggered()), this, SLOT(openNewFile()));

    m_menuBar->addMenu(m_menu);
    m_menuBar->show();
    setMenuBar(m_menuBar);
}

MainWindow::~MainWindow()
{

}
