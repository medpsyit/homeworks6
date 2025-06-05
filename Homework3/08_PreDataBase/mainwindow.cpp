#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    tableView = new QTableView();

    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    connect(ui->pb_clear, &QPushButton::clicked, this, &MainWindow::on_pb_clear_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;

    delete dataDb;
    delete dataBase;
    delete msg;

    delete tableView;
}

void MainWindow::on_act_addData_triggered()
{
    dataDb->exec();
}

void MainWindow::on_act_connect_triggered()
{

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");


       auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
       QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }

}

void MainWindow::on_pb_request_clicked()
{

    int genreId = ui->cb_category->currentIndex() + 1;

    dataBase->RequestToDB(genreId);

}

void MainWindow::on_pb_clear_clicked() {

    ui->tableView->setModel(new QSqlTableModel());

}


void MainWindow::ScreenDataFromDB(QSqlQueryModel *model, int typeRequest)
{

    switch (typeRequest) {
        case requestAllFilms: {

            ui->tableView->setModel(model);

            int columnCount = model->columnCount();
            for (int i = 0; i < columnCount; ++i) {
                ui->tableView->setColumnHidden(i, true);
            }

            ui->tableView->setColumnHidden(1, false);
            ui->tableView->setColumnHidden(2, false);
            break;
        }
        case requestComedy: {

            ui->tableView->setModel(model);

            ui->tableView->setColumnHidden(0, false);
            ui->tableView->setColumnHidden(1, false);
            break;
        }
        case requestHorrors: {

            ui->tableView->setModel(model);

            ui->tableView->setColumnHidden(0, false);
            ui->tableView->setColumnHidden(1, false);
            break;
        }
        default: {
            break;
        }
    }

}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}



