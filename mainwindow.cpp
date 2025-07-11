#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::conectarServidor);    // "Connect"
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::desconectarServidor); // "Disconnect"
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::atualizarListaMaquinas); // "Update"
    connect(ui->startButton,   &QPushButton::clicked, this, &MainWindow::iniciarColeta); // "Start"
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::pararColeta);   // "Stop"
}


void MainWindow::conectarServidor()
{
    QString ip = ui->lineEdit->text();

    socket->connectToHost(ip, 1234);
    if (socket->waitForConnected(3000)) {
        QMessageBox::information(this, "Conexão", "Conectado com sucesso!");
    } else {
        QMessageBox::warning(this, "Erro", "Falha na conexão com o servidor.");
    }
}

void MainWindow::desconectarServidor()
{
    if (socket->isOpen()) {
        socket->disconnectFromHost();
        QMessageBox::information(this, "Desconexão", "Desconectado do servidor.");
    }
}

void MainWindow::atualizarListaMaquinas()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write("list\r\n");
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        ui->listView->model()->deleteLater(); // Limpa modelo anterior, se necessário

        QStringList list;
        while (socket->bytesAvailable()) {
            QString linha = socket->readLine().trimmed();
            if (!linha.isEmpty()) {
                list.append(linha);
            }
        }

        // QStringListModel *modelo = new QStringListModel(list, this);
        // ui->listView->setModel(modelo);
    }
}

void MainWindow::iniciarColeta(){

}

void MainWindow::pararColeta(){

}

void MainWindow::coletarDados(){

    atualizarGrafico();
}

void MainWindow::atualizarGrafico(){

}

void MainWindow::atualizarIntervalo(int valor)
{
    // ui->lcdNumber->display(valor);
    // timer->setInterval(valor * 1000); // segundos para milissegundos
}

MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}
