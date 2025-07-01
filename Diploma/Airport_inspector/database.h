#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QtSql>

#include <QDebug>


#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

enum direction {
    arrival,
    departure
};

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void DisconnectFromDataBase(void);
    QSqlError GetLastError(void);
    void ConnectToDataBase(void);

    void RecordAirportsList(void);
    QStringList GetAirportsList(void);
    void RequestSchedule(QString &airportCode, int &direction, QString &date);

    QString GetAirportCode(QString &airportName);

    void GetYearModel(QString &airportCode);

    void GetMonthsModels(QString &airportCode);

signals:

   void sig_SendDataFromDB(QSqlQueryModel* model);
   void sig_SendStatusConnection(bool);

   void sig_SendYearModel(QSqlQueryModel *model);
   void sig_SendMonthsModels(const QList<QSqlQueryModel*> &models);

private:

    QSqlDatabase* dataBase;

    QMap<QString, QString> airportsList;

    QTableWidget* tableWidget;

};


#endif // DATABASE_H
