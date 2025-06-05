#include "dbdata.h"
#include "database.h"
#include "ui_dbdata.h"

#include <QPushButton>
#include <QAction>
#include <QIcon>

DbData::DbData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbData)
{
    ui->setupUi(this);

    data.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    QPushButton* auto_fill_button = ui->buttonBox->addButton("Автозаполнение", QDialogButtonBox::ActionRole);
    connect(auto_fill_button, &QPushButton::clicked, this, &DbData::on_buttonBox_autofill);

    QPushButton* clear_button = ui->buttonBox->addButton("Очистить", QDialogButtonBox::ActionRole);
    connect(clear_button, &QPushButton::clicked, this, &DbData::on_buttonBox_clear);

    ui->le_pass->setEchoMode(QLineEdit::Password);

    show_pass = ui->le_pass->addAction(QIcon("../08_PreDataBase/hide.png"), QLineEdit::TrailingPosition);
    show_pass->setToolTip("Показать пароль");
    connect(show_pass, &QAction::triggered, this, &DbData::check_pass);

}

DbData::~DbData()
{
    delete ui;
    delete show_pass;
}

void DbData::on_buttonBox_accepted()
{

    data[hostName] = ui->le_host->text();
    data[dbName] = ui->le_dbName->text();
    data[login] = ui->le_login->text();
    data[pass] = ui->le_pass->text();
    data[port] = ui->spB_port->text();

    emit sig_sendData(data);

}

void DbData::on_buttonBox_autofill() {

    ui->le_host->setText("981757-ca08998.tmweb.ru");
    ui->le_dbName->setText("netology_cpp");
    ui->le_login->setText("netology_usr_cpp");
    ui->le_pass->setText("CppNeto3");
    ui->spB_port->setValue(5432);

}

void DbData::on_buttonBox_clear() {

    ui->le_host->clear();
    ui->le_dbName->clear();
    ui->le_login->clear();
    ui->le_pass->clear();
    ui->spB_port->clear();

}

void DbData::check_pass() {
    if (show_pass->toolTip() == "Показать пароль") {
        show_pass->setIcon(QIcon("../08_PreDataBase/view.png"));
        show_pass->setToolTip("Скрыть пароль");
        ui->le_pass->setEchoMode(QLineEdit::Normal);
    } else {
        show_pass->setIcon(QIcon("../08_PreDataBase/hide.png"));
        show_pass->setToolTip("Показать пароль");
        ui->le_pass->setEchoMode(QLineEdit::Password);
    }
}
