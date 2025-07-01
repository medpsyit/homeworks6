#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();

    tableWidget = new QTableWidget();

}

DataBase::~DataBase()
{
    delete dataBase;

    delete tableWidget;
}

void DataBase::ConnectToDataBase(void)
{

    *dataBase = QSqlDatabase::addDatabase(POSTGRE_DRIVER, DB_NAME);

    dataBase->setHostName("981757-ca08998.tmweb.ru");
    dataBase->setDatabaseName("demo");
    dataBase->setUserName("netology_usr_cpp");
    dataBase->setPassword("CppNeto3");
    dataBase->setPort(5432);

    bool status;
    status = dataBase->open();

    if (status) {
            qDebug() << "База данных успешно подключена.";
        } else {
            qDebug() << "Ошибка подключения к базе данных:" << dataBase->lastError().text();
        }

    emit sig_SendStatusConnection(status);

}

void DataBase::DisconnectFromDataBase(void)
{

    *dataBase = QSqlDatabase::database(DB_NAME);
    dataBase->close();

}

void DataBase::RecordAirportsList(void) {
    QSqlQueryModel* airports = new QSqlQueryModel(this);

    QString query = QString("SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data");
    airports->setQuery(query, *dataBase);

    if (airports->lastError().isValid()) {
        qDebug() << "Ошибка в выполнении запроса: " << airports->lastError().text();
        return;
    }

    if (airports->rowCount() > 0) { // Проверка, что есть данные
        for (int row = 0; row < airports->rowCount(); ++row) {
            QSqlRecord record = airports->record(row);
            airportsList[record.value(0).toString()] = record.value(1).toString();
        }
    } else {
        qDebug() << "Данных нет.";
    }
}

QStringList DataBase::GetAirportsList(void) {
    QStringList list;
    foreach (const QString &airport, airportsList.keys()) {
        list << airport;
    }
    list.sort();
    return list;
}

QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}

void DataBase::RequestSchedule(QString &airportCode, int &direction, QString &date) {

    switch (direction) {
    case arrival: {

        QString query = QString("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                                "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                                "WHERE f.arrival_airport  = '%1' AND scheduled_arrival::date = date('%2')").arg(airportCode, date);

        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(query, *dataBase);

        if (model->lastError().isValid()) {
            qDebug() << "Ошибка в выполнении запроса: " << model->lastError().text();
            return;
        }

        model->setHeaderData(2, Qt::Horizontal, "Аэропорт отправления");

        emit sig_SendDataFromDB(model);
        break;
    }
    case departure: {
        QString query = QString("SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                                "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                                "WHERE f.departure_airport  = '%1' AND scheduled_departure::date = date('%2')").arg(airportCode, date);

        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(query, *dataBase);

        if (model->lastError().isValid()) {
            qDebug() << "Ошибка в выполнении запроса: " << model->lastError().text();
            return;
        }

        model->setHeaderData(2, Qt::Horizontal, "Аэропорт назначения");

        emit sig_SendDataFromDB(model);
        break;
    }
    default:
        break;
    }

}

QString DataBase::GetAirportCode(QString &airportName) {
    QString airportCode = airportsList.value(airportName);
    return airportCode;
}

void DataBase::GetYearModel(QString &airportCode) {

    QSqlQueryModel* yearGraphTest = new QSqlQueryModel(this);

    QString query = QString("SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
                            "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
                            "and ( departure_airport = '%1' or arrival_airport = '%1' ) group by \"Month\"").arg(airportCode);
    yearGraphTest->setQuery(query, *dataBase);

    if (yearGraphTest->lastError().isValid()) {
        qDebug() << "Ошибка в выполнении запроса: " << yearGraphTest->lastError().text();
        return;
    }

    emit sig_SendYearModel(yearGraphTest);
}

void DataBase::GetMonthsModels(QString &airportCode) {

    QList<QSqlQueryModel*> models;

    QDate dateFrom(2016, 9, 1);
    QDate dateTo(dateFrom.year(), dateFrom.month(), dateFrom.daysInMonth());

    for (int count = 0; count < 12; count++) {

        QSqlQueryModel* monthGraphTest = new QSqlQueryModel(this);

        QString query = QString("SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
                                "WHERE(scheduled_departure::date >= date('%2') and scheduled_departure::date <= date('%3')) "
                                "and ( departure_airport = '%1' or arrival_airport = '%1') GROUP BY \"Day\"").arg(airportCode, dateFrom.toString("yyyy-MM-dd"),
                                                                                                                  dateTo.toString("yyyy-MM-dd"));

        monthGraphTest->setQuery(query, *dataBase);

        if (monthGraphTest->lastError().isValid()) {
            qDebug() << "Ошибка в выполнении запроса: " << monthGraphTest->lastError().text();
            return;
        }

        models.append(monthGraphTest);
        dateFrom = dateFrom.addMonths(1);

        dateTo.setDate(dateFrom.year(), dateFrom.month(), dateFrom.daysInMonth());
    }

    emit sig_SendMonthsModels(models);
}

