#ifndef TABELLE_H
#define TABELLE_H

#include <QStandardItemModel>
#include <QTextStream>
#include <QList>
#include "mainwindow.h"


class tabelle
{
    Q_OBJECT
    void ListModel();
public:
    tabelle();
    QStandardItemModel* model;
};

#endif // TABELLE_H
