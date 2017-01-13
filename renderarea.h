#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QMultiMap>
#include <QString>
#include <QPair>
#include <QDateTime>

enum class FileAction{
    Open,
    Read,
    Write,
    Close
};

inline char toChar(FileAction f)
{
    switch (f)
    {
    case FileAction::Close:
        return 'C';
    case FileAction::Read:
        return 'R';
    case FileAction::Open:
        return 'O';
    case FileAction::Write:
        return 'W';
    }
}

using micros = unsigned long;
using FileEvent = QPair<FileAction, quint64>;
using EventList = QVector<FileEvent>;
struct NamedEventList{
    explicit NamedEventList(){}
    NamedEventList(const QString newFileName, const EventList newEvents){
        filename = newFileName;
        events = newEvents;
    }
    QString filename;
    EventList events;
};

using FileEventList = QVector<NamedEventList>;

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(FileEventList *eventData, quint64 lastTimeStamp, QWidget *parent = nullptr);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *wheel) Q_DECL_OVERRIDE;

private:
    int m_horizontalScrollPosition;
    QSize m_size;
    FileEventList *m_events;
    quint64 m_lastTimeStamp;
    int m_currentlySelected;

signals:

public slots:
};

#endif // RENDERAREA_H
