#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QWidget>
#include <QDateTime>
#include <QString>
#include <QListView>
#include <QTimerEvent>
#include <QSet>
#include <QTimer>

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
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::coletarDados);

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
    if (socket->state() == QAbstractSocket::ConnectedState) {
        timer->start(1000);  // Inicia a coleta a cada 1 segundo
        qDebug() << "Coleta iniciada";
        QMessageBox::information(this, "Coleta", "Coleta de dados iniciada.");
    } else {
        QMessageBox::warning(this, "Erro", "Conecte-se ao servidor antes de iniciar a coleta.");
    }
}

void MainWindow::pararColeta(){
    if (timer->isActive()) {
        timer->stop();  // Para o temporizador
        qDebug() << "Coleta parada";
        QMessageBox::information(this, "Coleta", "Coleta de dados parada.");
    } else {
        QMessageBox::information(this, "Aviso", "A coleta já está parada.");
    }
}

void MainWindow::coletarDados(){
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Não conectado ao servidor. Coleta cancelada.";
        return;
    }

    socket->write("get\r\n");  // Envia comando para obter o dado (ajuste conforme seu protocolo)
    socket->flush();

    if (!socket->waitForReadyRead(1000)) {  // Aguarda resposta do servidor
        qDebug() << "Nenhum dado recebido.";
        return;
    }

    while (socket->canReadLine()) {
        QString linha = socket->readLine().trimmed();  // Lê e remove espaços e \n
        bool ok;
        double valor = linha.toDouble(&ok);

        if (ok) {
            qDebug() << "Valor recebido:" << valor;
            atualizarGrafico();
        } else {
            qDebug() << "Valor inválido recebido:" << linha;
        }
    }
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
