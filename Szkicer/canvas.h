#ifndef CANVAS_H
#define CANVAS_H

#include <QtGui>
#include <QWidget>

class Canvas : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *event);
public:
    explicit Canvas(QWidget *parent = 0);

signals:

public slots:

};

#endif // CANVAS_H
