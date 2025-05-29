#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->vert_prog_button->setText("Vertical Progress Bar");
    ui->horiz_prog_button->setText("Horizontal Progress Bar");

    ui->vert_progress_bar->setOrientation(Qt::Vertical);
    ui->vert_progress_bar->setMinimum(0);
    ui->vert_progress_bar->setMaximum(10);
    ui->vert_progress_bar->setValue(0);
    ui->vert_progress_bar->setAlignment(Qt::AlignVCenter);
    ui->vert_progress_bar->setEnabled(false);

    ui->horiz_progress_bar->setOrientation(Qt::Horizontal);
    ui->horiz_progress_bar->setMinimum(0);
    ui->horiz_progress_bar->setMaximum(10);
    ui->horiz_progress_bar->setValue(0);
    ui->horiz_progress_bar->setAlignment(Qt::AlignCenter);
    ui->horiz_progress_bar->setEnabled(false);

    ui->color_prog_bar->addItems({ "RED", "YELLOW", "BLACK", "GREEN", "BLUE", "PINK", "WHITE", "GREY" });
    ui->color_prog_bar->setCurrentIndex(-1);

    ui->toggle_button->setText("Click Me");

    connect(ui->vert_prog_button, &QRadioButton::toggled, this, &MainWindow::toggle_bar);
    connect(ui->horiz_prog_button, &QRadioButton::toggled, this, &MainWindow::toggle_bar);

    connect(ui->toggle_button, &QPushButton::toggled, this, &MainWindow::increase_progress);

    connect(ui->color_prog_bar, &QComboBox::activated, this, &MainWindow::set_color);
}

void MainWindow::toggle_bar(bool checked) {
    if (sender() == ui->vert_prog_button) {
        if (checked) {
            ui->vert_progress_bar->setEnabled(true);
        }
        else {
            ui->vert_progress_bar->setEnabled(false);
        }
    } else if (sender() == ui->horiz_prog_button) {
        if (checked) {
            ui->horiz_progress_bar->setEnabled(true);
        }
        else {
            ui->horiz_progress_bar->setEnabled(false);
        }
    }
}

void MainWindow::increase_progress() {
    if (ui->horiz_prog_button->isChecked()) {
        int curr_value = ui->horiz_progress_bar->value();
        if (curr_value < 10) {
            curr_value++;
            ui->horiz_progress_bar->setValue(curr_value);
        } else {
            ui->horiz_progress_bar->setValue(0);
        }
    } else if (ui->vert_prog_button->isChecked()) {
        int curr_value = ui->vert_progress_bar->value();
        if (curr_value < 10) {
            curr_value++;
            ui->vert_progress_bar->setValue(curr_value);
        } else {
            ui->vert_progress_bar->setValue(0);
        }
    }
}

void MainWindow::set_color(){
    int curr_index = ui->color_prog_bar->currentIndex();
    switch(curr_index) {
    case 0:
        if (ui->vert_prog_button->isChecked()) {
            ui->vert_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: red; } QProgressBar { text-align: left; }");
        } else if (ui->horiz_prog_button->isChecked()){
        ui->horiz_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    }
        break;

    case 1:
        if (ui->vert_prog_button->isChecked()) {
            ui->vert_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: yellow; } QProgressBar { text-align: left; }");
        } else if (ui->horiz_prog_button->isChecked()){
        ui->horiz_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: yellow; }");
    }
        break;

    case 2:
        if (ui->vert_prog_button->isChecked()) {
            ui->vert_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: black; } QProgressBar { text-align: left; }");
        } else if (ui->horiz_prog_button->isChecked()){
        ui->horiz_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: black; }");
    }
        break;

    case 3:
        if (ui->vert_prog_button->isChecked()) {
            ui->vert_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: green; } QProgressBar { text-align: left; }");
        } else if (ui->horiz_prog_button->isChecked()){
        ui->horiz_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    }
        break;

    case 4:
        if (ui->vert_prog_button->isChecked()) {
            ui->vert_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: blue; } QProgressBar { text-align: left; }");
        } else if (ui->horiz_prog_button->isChecked()){
        ui->horiz_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: blue; }");
    }
        break;

    case 5:
        if (ui->vert_prog_button->isChecked()) {
            ui->vert_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: pink; } QProgressBar { text-align: left; }");
        } else if (ui->horiz_prog_button->isChecked()){
        ui->horiz_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: pink; }");
    }
        break;

    case 6:
        if (ui->vert_prog_button->isChecked()) {
            ui->vert_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: white; } QProgressBar { text-align: left; }");
        } else if (ui->horiz_prog_button->isChecked()){
        ui->horiz_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: white; }");
    }
        break;

    case 7:
        if (ui->vert_prog_button->isChecked()) {
            ui->vert_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: grey; } QProgressBar { text-align: left; }");
        } else if (ui->horiz_prog_button->isChecked()){
        ui->horiz_progress_bar->setStyleSheet("QProgressBar::chunk { background-color: grey; }");
    }
        break;

    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

