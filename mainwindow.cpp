#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QWidget>
#include <QDateTime>
#include <QString>
#include <QListView>
#include <QTimerEvent>
#include <QSet>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    model = new QStringListModel(this);
    ui->listView->setModel(model);

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Erro ao iniciar servidor TCP:" << tcpServer->errorString();
    } else {
        qDebug() << "Servidor escutando na porta 1234";
        connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::novaConexao);
    }

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::conectarServidor);    // "Connect"
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::desconectarServidor); // "Disconnect"
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::atualizarListaMaquinas); // "Update"
    connect(ui->startButton,   &QPushButton::clicked, this, &MainWindow::iniciarColeta); // "Start"
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::pararColeta);    // "Stop"
    connect(ui->listView, &QListView::clicked, this, &MainWindow::onMaquinaSelecionada);
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
void MainWindow::onMaquinaSelecionada(const QModelIndex &index)
{
    QString ipSelecionado = model->data(index, Qt::DisplayRole).toString();
    qDebug() << "IP selecionado:" << ipSelecionado;

    ui->label_2->setText("Selecionado: " + ipSelecionado);
}
void MainWindow::atualizarListaMaquinas()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write("list\r\n");
        socket->waitForBytesWritten(3000);
        if (!socket->waitForReadyRead(3000)) {
            qDebug() << " nenhum dado recebido.";
            return;
        }

        QStringList list;
        while (socket->bytesAvailable()) {
            QString linha = socket->readLine();
            if (!linha.isEmpty()) {
                list.append(linha);
                qDebug() << linha;
            }
        }

        model->setStringList(list);
    }
}
void MainWindow::novaConexao()
{
    QTcpSocket *cliente = tcpServer->nextPendingConnection();
    QString ip = cliente->peerAddress().toString();

    if (!listaIps.contains(ip)) {
        listaIps.insert(ip);
        model->setStringList(listaIps.values());
        qDebug() << "Novo IP conectado:" << ip;
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
