#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new TCPclient(this);

    ui->le_data->setEnabled(false);
    ui->pb_request->setEnabled(false);
    ui->lb_connectStatus->setText("Отключено");
    ui->lb_connectStatus->setStyleSheet("color: red");

    connect(client, &TCPclient::sig_Disconnected, this, [&]{

        ui->lb_connectStatus->setText("Отключено");
        ui->lb_connectStatus->setStyleSheet("color: red");
        ui->pb_connect->setText("Подключиться");
        ui->le_data->setEnabled(false);
        ui->pb_request->setEnabled(false);
        ui->spB_port->setEnabled(true);
        ui->spB_ip1->setEnabled(true);
        ui->spB_ip2->setEnabled(true);
        ui->spB_ip3->setEnabled(true);
        ui->spB_ip4->setEnabled(true);

    });

    connect(client, &TCPclient::sig_sendTime, this, &MainWindow::DisplayTime);
    connect(client, &TCPclient::sig_connectStatus, this, &MainWindow::DisplayConnectStatus);
    connect(client, &TCPclient::sig_sendStat, this, &MainWindow::DisplayStat);
    connect(client, &TCPclient::sig_sendFreeSize, this, &MainWindow::DisplayFreeSpace);
    connect(client, &TCPclient::sig_SendReplyForSetData, this, &MainWindow::SetDataReply);
    connect(client, &TCPclient::sig_Success, this, &MainWindow::DisplaySuccess);
    connect(client, &TCPclient::sig_Error, this, &MainWindow::DisplayError);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayTime(QDateTime time)
{
    ui->tb_result->append("Текущее время и дата на сервере: " + time.toString() + ". ");
}
void MainWindow::DisplayFreeSpace(uint32_t freeSpace)
{
    ui->tb_result->append("Свободного места на сервере: " + QString::number(freeSpace) + " bytes. ");
}
void MainWindow::SetDataReply(QString replyString)
{
    ui->tb_result->append("На сервер передано сообщение: \"" + ui->le_data->text() + "\". ");
}
void MainWindow::DisplayStat(StatServer stat)
{
    ui->tb_result->append("Статистика сервера:\n"
                          "Принято байт: " + QString::number(stat.incBytes) + ". \n"
                          "Передано байт: " + QString::number(stat.sendBytes) + ". \n"
                          "Принято пакетов: " + QString::number(stat.revPck) + ". \n"
                          "Передано пакетов: " + QString::number(stat.sendPck) + ". \n"
                          "Время работы сервера: " + QString::number(stat.workTime) + " sec. \n"
                          "Количество подключенных клиентов: " + QString::number(stat.clients) + ". ");
}
void MainWindow::DisplayError(uint16_t error)
{
    switch (error) {
    case ERR_NO_FREE_SPACE:
    {
        ui->tb_result->append("Ошибка! Для данных нет свободного места на сервере!");
        break;
    }
    case ERR_NO_FUNCT:
    {
        ui->tb_result->append("Ошибка! Данный функционал в текущей версии не реализован!");
        break;
    }
    default:
        break;
    }
}

void MainWindow::DisplaySuccess(uint16_t typeMess)
{
    switch (typeMess) {
    case CLEAR_DATA: {
        ui->tb_result->append("Память сервера очищена!");
        break;
    }
    default:
        break;
    }

}

void MainWindow::DisplayConnectStatus(uint16_t status)
{

    if(status == ERR_CONNECT_TO_HOST){

        ui->tb_result->append("Ошибка подключения к порту: " + QString::number(ui->spB_port->value()));

    }
    else{
        ui->lb_connectStatus->setText("Подключено");
        ui->lb_connectStatus->setStyleSheet("color: green");
        ui->pb_connect->setText("Отключиться");
        ui->spB_port->setEnabled(false);
        ui->pb_request->setEnabled(true);
        ui->spB_ip1->setEnabled(false);
        ui->spB_ip2->setEnabled(false);
        ui->spB_ip3->setEnabled(false);
        ui->spB_ip4->setEnabled(false);
    }

}

void MainWindow::on_pb_connect_clicked()
{
    if(ui->pb_connect->text() == "Подключиться"){

        uint16_t port = ui->spB_port->value();

        QString ip = ui->spB_ip4->text() + "." +
                     ui->spB_ip3->text() + "." +
                     ui->spB_ip2->text() + "." +
                     ui->spB_ip1->text();

        client->ConnectToHost(QHostAddress(ip), port);

    }
    else{

        client->DisconnectFromHost();
    }
}

void MainWindow::on_pb_request_clicked()
{

   ServiceHeader header;

   header.id = ID;
   header.status = STATUS_SUCCESS;
   header.len = 0;

   switch (ui->cb_request->currentIndex()){

       //Получить время
    case 0: {
        header.idData = 100;
        break;
    }
       //Получить свободное место
    case 1: {
        header.idData = 101;
        break;
    }
       //Получить статистику
    case 2: {
        header.idData = 102;
        break;
    }
       //Отправить данные
    case 3: {
        header.idData = 200;
        QString str = ui->le_data->text();
        header.len = str.size();
        client->SendData(header, str);
        break;
    }
       //Очистить память на сервере
    case 4: {
        header.idData = 201;
        break;
    }
    default: {
        ui->tb_result->append("Такой запрос не реализован в текущей версии");
        break;
    }

   }

   if (header.idData != 200) {
       client->SendRequest(header);
   }

}

void MainWindow::on_cb_request_currentIndexChanged(int index)
{
    //Разблокируем поле отправления данных только когда выбрано "Отправить данные"
    if(ui->cb_request->currentIndex() == 3){
        ui->le_data->setEnabled(true);
    }
    else{
        ui->le_data->setEnabled(false);
    }
}

