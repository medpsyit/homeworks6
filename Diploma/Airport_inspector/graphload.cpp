#include "graphload.h"
#include "ui_graphload.h"

GraphLoad::GraphLoad(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphLoad)
{
    ui->setupUi(this);

    ui->pb_close_graph->setText("Закрыть");

    monthsGraph = new QChart();
    yearGraph = new QChart();
    graphView = new QChartView(monthsGraph);
    yearGraphView = new QChartView(yearGraph);

    RecordMonths();

    connect(ui->pb_close_graph, &QPushButton::clicked, this, &GraphLoad::on_pb_close_clicked);

    ui->year_graph_layout->addWidget(yearGraphView);
    ui->graph_layout->addWidget(graphView);

    connect(ui->cb_months, &QComboBox::currentIndexChanged, this, &GraphLoad::MonthsGraphsCreate);

}

GraphLoad::~GraphLoad()
{
    delete ui;

    delete monthsGraph;
    delete yearGraph;
    delete graphView;
    delete yearGraphView;
}

void GraphLoad::on_pb_close_clicked() {

    this->close();
}

void GraphLoad::RecordMonths() {

    QStringList months;
    months << "January" << "February" << "March"
           << "April" << "May" << "June"
           << "July" << "August" << "September"
           << "October" << "November" << "December";
    ui->cb_months->addItems(months);
    ui->cb_months->setCurrentIndex(0);
}

void GraphLoad::YearGraphCreate(QSqlQueryModel *model) {

    QList<int> flights_year;

    if (model != nullptr && model->rowCount() > 0) {
        for (int row = 0; row < model->rowCount(); ++row) {
            QSqlRecord record = model->record(row);

            flights_year.append(record.value(0).toInt());
        }

        yearGraph->setTitle("Годовой график загруженности");

        QBarSet *flights = new QBarSet("flights");

        for (const auto curr : flights_year) {
            flights->append(curr);
        }

        QStackedBarSeries *Series = new QStackedBarSeries();
        Series->append(flights);

        yearGraph->addSeries(Series);
        yearGraph->setAnimationOptions(QChart::SeriesAnimations);

        QStringList Months;
        Months << "September" << "October" << "November" << "December"
               << "January" << "February" << "March" << "April"
               << "May" << "June" << "July" << "August";

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(Months);
        axisX->setTitleText("Month");
        yearGraph->addAxis(axisX, Qt::AlignBottom);

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Values");
        yearGraph->addAxis(axisY, Qt::AlignLeft);

        Series->attachAxis(axisX);
        Series->attachAxis(axisY);

        yearGraph->legend()->setVisible(false);
        yearGraph->legend()->setAlignment(Qt::AlignBottom);

        yearGraphView->setRenderHint(QPainter::Antialiasing);

        yearGraphView->update();

    } else {
        qDebug() << "Данных нет.";
    }

}

void GraphLoad::MonthsGraphsCreate() {

    ClearChart(monthsGraph);
    graphView->update();

    monthsGraph->setTitle("Загруженность за месяц");

    QSqlQueryModel *model = nullptr;
    QList<QPointF> forGraph;

    switch (ui->cb_months->currentIndex()) {
    case 0: {
        model = models[4];
        break;
    }
    case 1: {
        model = models[5];
        break;
    }
    case 2: {
        model = models[6];
        break;
    }
    case 3: {
        model = models[7];
        break;
    }
    case 4: {
        model = models[8];
        break;
    }
    case 5: {
        model = models[9];
        break;
    }
    case 6: {
        model = models[10];
        break;
    }
    case 7: {
        model = models[11];
        break;
    }
    case 8: {
        model = models[0];
        break;
    }
    case 9: {
        model = models[1];
        break;
    }
    case 10: {
        model = models[2];
        break;
    }
    case 11: {
        model = models[3];
        break;
    }
    default:
        break;
    }

    int MaxDay {0};
    int MinValue{100000};
    int MaxValue{0};

    if (model != nullptr && model->rowCount() > 0) {
        for (int row = 0; row < model->rowCount(); ++row) {
            QSqlRecord record = model->record(row);

            if (record.value(0).toInt() > MaxValue) {
                MaxValue = record.value(0).toInt();
            }

            if (record.value(0).toInt() < MinValue) {
                MinValue = record.value(0).toInt();
            }

            QPointF point(row + 1, record.value(0).toInt());
            forGraph.append(point);
            MaxDay++;
        }
    }

    MaxValue += 1;
    if (MinValue != 0){
        MinValue -= 1;
    }

    QLineSeries *series = new QLineSeries();
    series->append(forGraph);

    monthsGraph->addSeries(series);

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    axisX->setTitleText("Days");
    axisX->setRange(1, MaxDay);

    axisY->setTitleText("Values");
    axisY->setRange(MinValue, (MaxValue));

    monthsGraph->addAxis(axisX, Qt::AlignBottom);
    monthsGraph->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    monthsGraph->legend()->setVisible(false);

    graphView->setRenderHint(QPainter::Antialiasing);

    graphView->update();

}

void GraphLoad::ClearChart(QChart *chart) {

    auto seriesList = chart->series();
        for (auto *series : seriesList) {

            auto *barSeries = qobject_cast<QAbstractBarSeries*>(series);
            if (barSeries) {
                auto barSets = barSeries->barSets();
                for (auto *set : barSets) {
                    delete set;
                }
            }

            chart->removeSeries(series);
            delete series;
        }

    auto axes = chart->axes();
    for (auto *axis : axes) {
        chart->removeAxis(axis);
        delete axis;
    }
}

void GraphLoad::SetModels(const QList<QSqlQueryModel*> &curr_models) {
    models = curr_models;
}

void GraphLoad::closeEvent(QCloseEvent *event)
{
    ui->cb_months->setCurrentIndex(0);

    ClearChart(yearGraph);
    ClearChart(monthsGraph);

    yearGraphView->update();
    graphView->update();

    QWidget::closeEvent(event);
}
