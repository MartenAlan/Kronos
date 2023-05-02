#ifndef DAY_H
#define DAY_H
#include <QList>
#include <QDate>

struct TimeInterval
{
    QTime begin;
    QTime end;
    int minutes;
    int minutesRaw;
    QString type;
    int typeID; int errorID;
    void calculateMinutes();
    void getMinutesRaw();
    TimeInterval(QString type);
    TimeInterval(QTime begin, QTime end);
    TimeInterval(QTime begin);
};

class Day
{
public:
    Day();
    QList<TimeInterval> times;
    QDate date;
    int dayResultsID; int journalID;
    int saldo;
    int soll;
    int ntges;
    void calculateMinutes();
};

#endif // DAY_H

