#include "canvas.h"

Canvas::Canvas(QWidget *parent) :
    QWidget(parent)
{
}


void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(0, 0, this->width(), this->height(), QBrush(Qt::white));

    qreal div = 30;

    painter.save();
    painter.translate(this->width() / 2, this->height() / 2);

    int verticals = ceil(this->width() / 2.0 / div);
    for (int i = -verticals; i < verticals; ++i)
    {
        qreal x = div * i;
        painter.setPen(QColor(200, 200, 200));
        painter.drawLine(x, -this->height() / 2, x, this->height() / 2);
        painter.setPen(Qt::black);
        if (i != 0)
        {
            painter.drawText(x - div/2, 2, div, div, Qt::AlignHCenter, QString::number(i));
        }
    }

    int horizontals = ceil(this->height() / 2.0 / div);
    for (int i = -horizontals; i < horizontals; ++i)
    {
        qreal y = div * i;
        painter.setPen(QColor(200, 200, 200));
        painter.drawLine(-this->width() / 2, y, this->width() / 2, y);
        painter.setPen(Qt::black);
        if (i != 0)
        {
            painter.drawText(2, y - div/2, div, div, Qt::AlignVCenter, QString::number(-i));
        }
    }

    painter.restore();

    painter.setPen(Qt::black);

    painter.drawLine(0, this->height() / 2, this->width(), this->height() / 2);
    painter.drawLine(this->width(), this->height() / 2, this->width() - 5, this->height() / 2 - 4.5);
    painter.drawLine(this->width(), this->height() / 2, this->width() - 5, this->height() / 2 + 4.5);

    painter.drawLine(this->width() / 2, 0, this->width() / 2, this->height());
    painter.drawLine(this->width() / 2, 0, this->width() / 2 - 4.5, 5);
    painter.drawLine(this->width() / 2, 0, this->width() / 2 + 4.5, 5);

    painter.translate(this->width() / 2, this->height() / 2);

    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    bool first = true;

    for (int i = -this->width() / 2; i < this->width() / 2; ++i)
    {
        qreal x = i / div;
        qreal y = x*x*x*x*x*x-x*x*x*x*x-x*x*x*x+x*x*x-x*x+x+1;
        QPoint point(x * div + 0.5, -y * div + 0.5);

        if (first)
            path.moveTo(point);
        else
            path.lineTo(point);

        first = false;

    }

    painter.drawPath(path);

}
