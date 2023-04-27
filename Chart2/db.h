#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QDebug>
#include <QDate>
#include <QList>


class DB
{
public:
    DB();
    QSqlDatabase db;
    QVector<int> getList(int a, QDate start, QDate end);
    int getHO(int a,QDate start, QDate end);
    QVector<int> getZeit(QDate datum);
    QVector<QString> getTyp(QDate datum);
};

#endif // DB_H
