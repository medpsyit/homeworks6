#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lb_connectStatus->setStyleSheet("color:red");

    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    tableView = new QTableView();

    graphs = new GraphLoad(this);

    SetDefault();

    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    dataBase->ConnectToDataBase();

    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenSchedule);
    connect(ui->rb_arrival, &QRadioButton::clicked, this, &MainWindow::on_rb_checked);
    connect(ui->rb_departure, &QRadioButton::clicked, this, &MainWindow::on_rb_checked);
    connect(ui->pb_schedule, &QPushButton::clicked, this, &MainWindow::on_pb_schedule_clicked);
    connect(ui->pb_get_graphs, &QPushButton::clicked, this, &MainWindow::on_pb_get_graphs_clicked);

    connect(dataBase, &DataBase::sig_SendYearModel, graphs, &GraphLoad::YearGraphCreate);
    connect(dataBase, &DataBase::sig_SendMonthsModels, graphs, &GraphLoad::SetModels);

}

MainWindow::~MainWindow()
{
    delete ui;

    delete dataBase;
    delete msg;

    delete tableView;

    delete graphs;
}

void MainWindow::SetDefault() {

    ui->pb_schedule->setEnabled(false);
    ui->pb_get_graphs->setEnabled(false);
    ui->date_select->setEnabled(false);
    ui->rb_arrival->setEnabled(false);
    ui->rb_departure->setEnabled(false);

    ui->pb_schedule->setText("Получить расписание");
    ui->pb_get_graphs->setText("Открыть график загруженности");
    ui->date_select->setMinimumDate(QDate(2016, 8, 15));
    ui->date_select->setMaximumDate(QDate(2017, 9, 14));
    ui->rb_arrival->setText("Прилёт");
    ui->rb_departure->setText("Вылет");

}

void MainWindow::RecordAirports() {
    dataBase->RecordAirportsList();
    QStringList sortedList = dataBase->GetAirportsList();
    ui->cb_airports->addItems(sortedList);
}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->lb_connectStatus->setText("Подключено");
        ui->lb_connectStatus->setStyleSheet("color:green");

        RecordAirports();
        ui->date_select->setEnabled(true);
        ui->rb_arrival->setEnabled(true);
        ui->rb_departure->setEnabled(true);
        ui->pb_get_graphs->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase();
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_connectStatus->setText("Отключено");
        ui->lb_connectStatus->setStyleSheet("color:red");
        msg->exec();
        QTimer::singleShot(5000, this, [&](){
            dataBase->ConnectToDataBase();
        });
    }

}

void MainWindow::ScreenSchedule(QSqlQueryModel *model) {

    model->setHeaderData(0, Qt::Horizontal, "Номер рейса");
    model->setHeaderData(1, Qt::Horizontal, "Время вылета");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_pb_schedule_clicked(){

    QString airport = ui->cb_airports->currentText();
    airport = dataBase->GetAirportCode(airport);
    QString date = ui->date_select->date().toString("yyyy-MM-dd");
    int direction;
    if (ui->rb_arrival->isChecked()) {
        direction = arrival;
    } else if (ui->rb_departure->isChecked()) {
        direction = departure;
    }

    dataBase->RequestSchedule(airport, direction, date);
}

void MainWindow::on_rb_checked(){
    ui->pb_schedule->setEnabled(true);
}

void MainWindow::on_pb_get_graphs_clicked() {

    QString airport = ui->cb_airports->currentText();
    airport = dataBase->GetAirportCode(airport);

    dataBase->GetYearModel(airport);

    dataBase->GetMonthsModels(airport);

    graphs->MonthsGraphsCreate();

    graphs->exec();

}
