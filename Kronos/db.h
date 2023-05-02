#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QDebug>
#include <QDate>
#include <QList>
#include <journal.h>
#include <day.h>

class DB
{
public:
    DB();
    QSqlDatabase db;
    QVector<int> getList(int a, QDate start, QDate end);
    int getHO(int a,QDate start, QDate end);
    QVector<int> getZeit(QDate datum);
    QVector<QString> getTyp(QDate datum);

    void createTables();
    void execQueryWithoutResultSet(QString queryString);
    void insertNewDay(Day);
    int insertNewJournal(Journal *);
    int insertNewDayResults(Day);
    void fillTypesTable();
    void fillErrorTable();
    static QMap<QString,QString> getTypeMap();
    static QMap<QString, QString> getErrorMap();
};

#endif // DB_H
