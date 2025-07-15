#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTcpServer>
#include <QStringListModel>
#include <QSet>

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
    void onMaquinaSelecionada(const QModelIndex &index);
    void atualizarIntervalo(int valor);
    void novaConexao();
private:
    Ui::MainWindow *ui;
    QTcpServer *tcpServer;
    QTcpSocket *socket;
    QStringListModel *model;
    QStringList list;
    QSet<QString> listaIps;

    void atualizarGrafico();
};

#endif // MAINWINDOW_H
