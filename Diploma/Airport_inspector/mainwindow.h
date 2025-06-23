#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QTimer>
#include "database.h"
#include "graphload.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void ReceiveStatusConnectionToDB(bool status);

    void RecordAirports();

    void ScreenSchedule(QSqlQueryModel *model);

private slots:
    void on_pb_schedule_clicked();

signals:

private:

    Ui::MainWindow *ui;
    DataBase* dataBase;
    QMessageBox* msg;

    QTableView* tableView;

    GraphLoad* graphs;

    void SetDefault();

    void on_rb_checked();
    void on_pb_get_graphs_clicked();
};
#endif // MAINWINDOW_H
