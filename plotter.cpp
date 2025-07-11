#include "plotter.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QDebug>

Plotter::Plotter(QWidget *parent)
    : QWidget{parent}{
    r = 255;
    g = 255;
    b = 200;
}

void Plotter::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    QBrush brush;
    QPen pen;

    painter.setRenderHint(QPainter::Antialiasing);

    brush.setColor(QColor(r,g,b));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(rect());


    pen.setColor(QColor(255,0,0));
    pen.setWidth(2);
    painter.setPen(pen);

    //if(data.isEmpty()) return;

    int w = width();
    int h = height();

}

void Plotter::setData(const QVector<QPair<qint64, double>>& points) {
    data = points;
    repaint();
}

void Plotter::setTempo(int tempo){
    //  qDebug() << amp;
    this->tempo = tempo;
    repaint();
}


void Plotter::mudaCor(int r, int g, int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    repaint();
}
