#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();

    simpleQuery = new QSqlQuery();
    tableWidget = new QTableWidget();

}

DataBase::~DataBase()
{
    delete dataBase;

    delete simpleQuery;
    delete tableWidget;
}

void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    bool status;
    status = dataBase->open();

    if (status) {
            qDebug() << "База данных успешно подключена.";
        } else {
            qDebug() << "Ошибка подключения к базе данных:" << dataBase->lastError().text();
        }

    emit sig_SendStatusConnection(status);

}

void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}

QSqlQueryModel* DataBase::genre_filter(const QString& genre, QSqlDatabase* database) {
    QSqlQueryModel* queryModel = new QSqlQueryModel(this);

    QString query = QString("SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id "
                            "JOIN category c on c.category_id = fc.category_id WHERE c.name = '%1'").arg(genre);

    queryModel->setQuery(query, *database);
    queryModel->setHeaderData(0, Qt::Horizontal, tr("Название"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Описание"));

    return queryModel;
}

void DataBase::RequestToDB(int request)
{

    switch (request) {
        case requestAllFilms: {
            QSqlTableModel* tableModel = new QSqlTableModel(this, *dataBase);

            tableModel->setTable("film");
            tableModel->select();
            tableModel->setHeaderData(1, Qt::Horizontal, tr("Название"));
            tableModel->setHeaderData(2, Qt::Horizontal, tr("Описание"));

            emit sig_SendDataFromDB(tableModel, request);
            break;
        }
        case requestComedy: {

            auto model = genre_filter("Comedy", dataBase);

            emit sig_SendDataFromDB(model, request);
            break;
        }

        case requestHorrors: {

            auto model = genre_filter("Horror", dataBase);

            emit sig_SendDataFromDB(model, request);
            break;
        }
    }
}

QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
