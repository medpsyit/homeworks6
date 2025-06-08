#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

    connect(udpWorker, &UDPworker::sig_sendDataToGUI, this, &MainWindow::DisplayText);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_sent_clicked(){

    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

    udpWorker->text_on = true;

    outStr << ui->te_sent->toPlainText();

    udpWorker->SendDatagram(dataToSend);
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
    udpWorker->time_on = true;
}


void MainWindow::DisplayText(qint64 bytes, QHostAddress sender, QString text) {
    ui->te_result->append("Получено сообщение: \"" + text + "\". От: " + sender.toString()
                          + ". Размер: " + QString::number(bytes) + " байт.");
    udpWorker->text_on = false;
}

void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
    udpWorker->time_on = false;
}

