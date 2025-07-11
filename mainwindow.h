#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    // void tcpConnect();
    void conectarServidor();
    void desconectarServidor();
    void atualizarListaMaquinas();
    void iniciarColeta();
    void pararColeta();
    void coletarDados();
    void atualizarIntervalo(int valor);
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    //  QTimer *timer;
    //  QVector<QPair<qint64, double>> dados;

    void atualizarGrafico();
};

#endif // MAINWINDOW_H
