#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject(parent), q_timer {new QTimer(this)}
{
    q_timer->setInterval(100);

    connect(q_timer, &QTimer::timeout, this, &Stopwatch::on_timeout);
}

QTime Stopwatch::q_time_record() {
    return t;
}

QTime Stopwatch::q_time_lap() {
    return l_t;
}

int Stopwatch::get_lap_num(){
    return lap_count;
}

void Stopwatch::start() {
    q_timer->start();
}

void Stopwatch::stop() {
    q_timer->stop();
}

void Stopwatch::clear() {
    lap_count = 1;
    t.setHMS(0,0,0,0);
    l_t.setHMS(0,0,0,0);
}

void Stopwatch::save_lap() {
    lap_count++;
    l_t.setHMS(0,0,0,0);
}

void Stopwatch::on_timeout() {

    t = t.addMSecs(100);
    l_t = l_t.addMSecs(100);

    emit sig_time();
}
