#ifndef GRAPH_H
#define GRAPH_H


#include <QDialog>
#include <QtCharts>
#include "database.h"

namespace Ui {
class GraphLoad;
}

class GraphLoad : public QDialog
{
    Q_OBJECT

public:
    explicit GraphLoad(QWidget *parent = nullptr);
    ~GraphLoad();

signals:

private slots:

public slots:
    void YearGraphCreate(QSqlQueryModel *model);

    void MonthsGraphsCreate();

    void SetModels(const QList<QSqlQueryModel*> &curr_models);

private:
    Ui::GraphLoad *ui;

    QChart *monthsGraph;
    QChart *yearGraph;

    QChartView *graphView;
    QChartView *yearGraphView;

    QList<QSqlQueryModel*> models;

    void on_pb_close_clicked();

    void on_cb_months_changed();

    void RecordMonths();

    void ClearChart(QChart *chart);

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // GRAPH_H
