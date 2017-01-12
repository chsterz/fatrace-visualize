#include "renderarea.h"
#include <QDebug>

#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>

#include <climits>

namespace Design
{
    static QColor BackgroundGray = QColor(242,242,242,255);
    static QColor LibraryGreen = QColor(78, 154, 6, 255);
    static QColor QmlBlue = QColor(32, 74, 135, 255);
    static QColor ReadActionRed = QColor(239, 41, 41, 255);
    static QColor TimeLineGray = QColor(186, 189, 182, 255);
    static QColor TimeLineGraySecondary = QColor(211, 215, 207, 255);
}

RenderArea::RenderArea(FileEventList *eventData, quint64 lastTimeStamp, QWidget *parent)
    : QWidget(parent),
      m_events(eventData),
      m_lastTimeStamp(lastTimeStamp)
{
    QPalette palette;
    palette.setColor(QPalette::Background, Design::BackgroundGray);
    setPalette(palette);
    setAutoFillBackground(true);
    setMouseTracking(true);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(1800, 2 * m_events->size() + 40);
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    int ypos = event->pos().y();
    m_currentlySelected = ypos/2;
    update();
}

void RenderArea::paintEvent(QPaintEvent *)
{
    int offset = 0;
    float unit = width() / static_cast<float>(m_lastTimeStamp);

    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing, true);
    painter.setFont(QFont(painter.font().family(), 11));
    QFontMetrics fm(painter.font());

    //Draw rough timelines
    painter.setPen(Design::TimeLineGray);
    QPen pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);
    for(int i = 1; i < m_lastTimeStamp/1000/1000; i++)
    {
        painter.drawLine(QLine(1000 * 1000 * unit * i, 0, 1000 * 1000 * unit * i, height()));
    }

    //Draw finegrained timelines
    painter.setPen(Design::TimeLineGraySecondary);
    for(int i = 1; i < m_lastTimeStamp/1000/100; i++)
    {
        painter.drawLine(QLine(1000 * 100 * unit * i, 0, 1000 * 100 * unit * i, height()));
    }

    //Draw Time Labels
    painter.setPen(Design::TimeLineGray);
    for (int i = 1; i < m_lastTimeStamp/1000/1000; i++)
    {
        for (int j = 1000; j < height(); j += 1000)
            painter.drawText(QPoint(1000 * 1000 * unit * i + 4, j), QString("%1\u2009s").arg(i));
    }


    //Draw the events
    for (int i=0; i<m_events->size(); i++)
    {
        const NamedEventList &fileEvents = m_events->at(i);
        QDebug deb = qDebug();

        float xOpen = std::numeric_limits<float>::quiet_NaN();

        if (fileEvents.filename.contains(".so"))
            painter.setPen(Design::LibraryGreen);
        else if (fileEvents.filename.contains(".qml"))
            painter.setPen(Design::QmlBlue);
        else if (fileEvents.filename.contains(".png"))
            painter.setPen(Qt::darkCyan);
        else
            painter.setPen(Qt::gray);

        for (const auto &event: fileEvents.events)
        {
            if (event.first == FileAction::Open)
            {
                xOpen = event.second * unit;
                continue;
            }
            if (event.first == FileAction::Close or (xOpen != xOpen) )
            {
                if (m_currentlySelected == i)
                {
                    painter.setBrush(QBrush(painter.pen().color()));
                    painter.drawRect(QRect(xOpen, offset, event.second*unit+1.0f - xOpen, fm.height()));
                }
                else
                    painter.setBrush(QBrush(painter.pen().color()));
                    painter.drawRect(QRect(xOpen, offset, event.second*unit+1.0f - xOpen, 2));
                xOpen = std::numeric_limits<float>::quiet_NaN();
                continue;
            }
            if( event.first == FileAction::Read)
                continue;
            if (xOpen == xOpen)
            {
                painter.drawLine(QLine(xOpen, offset, width(), offset));
            }
        }
        for (const auto &event: fileEvents.events)
        {
            painter.setPen(Design::ReadActionRed);
            if (event.first == FileAction::Read)
            {
                if (m_currentlySelected == i)
                {
                    painter.setBrush(QBrush(painter.pen().color()));
                    painter.drawRect(QRect(event.second*unit, offset, 1, fm.height()));
                }
                else
                {
                    painter.drawRect(QRect(event.second*unit, offset, 1, 2));
                }
            }
        }
        if (m_currentlySelected == i)
        {
            painter.setBrush(Qt::transparent);
            painter.setPen(Qt::black);
            painter.drawRect(QRect(0, offset+1, width(), fm.height()-2));
            painter.setPen(Qt::black);
            bool isLeft = fileEvents.events.first().second*unit < width()/2;
            QRect rect = QRect(isLeft? width()/2 : 0, offset, width()/2-4, fm.height());
            painter.drawText(rect, Qt::AlignLeft ,fileEvents.filename);
            offset += fm.height();
        }
        else
            offset += 2;
    }

}
