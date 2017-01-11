#include "renderarea.h"
#include <QDebug>

#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>

#include <climits>

RenderArea::RenderArea(FileEventList *eventData, quint64 lastTimeStamp, QWidget *parent)
    : QWidget(parent),
      m_events(eventData),
      m_lastTimeStamp(lastTimeStamp)
{
    setBackgroundRole(QPalette::Base);
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
    painter.setFont(QFont(painter.font().family(), 8));
    QFontMetrics fm(painter.font());

    for (int i=0; i<m_events->size(); i++)
    {
        const NamedEventList &fileEvents = m_events->at(i);
        QDebug deb = qDebug();

        float xOpen = std::numeric_limits<float>::quiet_NaN();


        if (fileEvents.filename.contains(".so"))
            painter.setPen(Qt::darkGreen);
        else if (fileEvents.filename.contains(".qml"))
            painter.setPen(Qt::darkBlue);
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
            painter.setPen(Qt::red);
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
