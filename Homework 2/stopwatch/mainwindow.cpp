#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), stpwch {new Stopwatch(this)}
{
    ui->setupUi(this);
    ui->curr_time->setText(stpwch->q_time_record().toString("hh:mm:ss.zzz"));
    ui->start_stop->setText("START");
    ui->clear_time->setText("CLEAR");
    ui->record_lap->setText("LAP");

    ui->record_lap->setEnabled(false);

    connect(stpwch, &Stopwatch::sig_time, this, &MainWindow::update_timer);

    connect(ui->start_stop, &QPushButton::clicked, this, [&](){
        if (start) {
            ui->record_lap->setEnabled(true);
            start = false;
            ui->start_stop->setText("STOP");
            stpwch->start();
        } else {
            ui->record_lap->setEnabled(false);
            start = true;
            ui->start_stop->setText("START");
            stpwch->stop();
        }
    });

    connect(ui->clear_time, &QPushButton::clicked, this, [&]() {
        stpwch->clear();
        ui->curr_time->setText(stpwch->q_time_record().toString("hh:mm:ss.zzz"));
        ui->laps->clear();
    });

    connect(ui->record_lap, &QPushButton::clicked, this, [&](){
        int num = stpwch->get_lap_num();
        QString lap = QString::number(num) + " LAP: " + stpwch->q_time_lap().toString("hh:mm:ss.zzz");
        ui->laps->append(lap);
        stpwch->save_lap();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_timer() {
    ui->curr_time->setText(stpwch->q_time_record().toString("hh:mm:ss.zzz"));
}
