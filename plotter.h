#ifndef PLOTTER_H
#define PLOTTER_H
#include <QWidget>

class Plotter : public QWidget
{
    Q_OBJECT
private:
    float tempo;
    int r, g, b;
    QVector<QPair<qint64, double>> data;
public:
    explicit Plotter(QWidget *parent = nullptr);

public slots:
    void setData(const QVector<QPair<qint64, double>> &points);
    void paintEvent(QPaintEvent *event);
    void setTempo(int tempo);
    void mudaCor(int r, int g, int b);
};


#endif // PLOTTER_H
