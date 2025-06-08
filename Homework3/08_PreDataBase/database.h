#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QtSql>

#include <QDebug>


#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

#define NUM_DATA_FOR_CONNECT_TO_DB 5

enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

enum requestType{

    requestAllFilms = 1,
    requestComedy   = 2,
    requestHorrors  = 3

};


class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(int request);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);

signals:

   void sig_SendDataFromDB(QSqlQueryModel* model, int typeR);
   void sig_SendStatusConnection(bool);



private:

    QSqlDatabase* dataBase;

    QSqlQuery* simpleQuery;
    QTableWidget* tableWidget;

    QSqlQueryModel* genre_filter(const QString& genre, QSqlDatabase* database);
};

#endif // DATABASE_H
