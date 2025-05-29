#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QTimer>
#include <QTime>

class Stopwatch : public QObject
{
    Q_OBJECT

public:
    explicit Stopwatch(QObject *parent = nullptr);

    QTime q_time_record();
    QTime q_time_lap();
    int get_lap_num();

signals:
    void sig_time();

protected:
    QTime t {0,0};
    QTime l_t {0,0};

    int lap_count {1};

public slots:
    void start();
    void stop();
    void clear();
    void save_lap();

private:
    QTimer *q_timer;

private slots:
    void on_timeout();
};

#endif // STOPWATCH_H
