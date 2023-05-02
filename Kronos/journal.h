#ifndef JOURNAL_H
#define JOURNAL_H
#include <QStringList>
#include <QDate>
#include <day.h>

class Journal
{
public:
    Journal();
    QMap <QString, int> indexes;
    QList<QStringList> journaltext;
    QList<Day> days;
    QDate datum;
    QDateTime druckdatum;
    QString getCell(int row, int column);
    void getMetaInfos();
    void getDayInfos();
    void getMonthInfos();
    void setIndexesTo33();
    void setIndexesTo36();
    QString getType(QString fz);

    int diffnt; int ntges; int saldo; int soll;
    int urlalt;int urlrst;int urlakt;int urlges;

};

#endif // JOURNAL_H
