#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QCategoryAxis>
#include <QString>
#include <QDateTimeAxis>
#include <QDesktopServices>
#include "db.h"


class Chart
{
public:
    QChartView * createPieChart(int i, QDate start, QDate end);
    QChartView * createLineChart(QDate start, QDate end);
    QChartView * Chart_HO_non_static(QDate start, QDate end);
    DB db;
    QVector<int> TabelleMinuten(QDate datum);
    QVector<QString> TabelleTyp(QDate datum);

};

#endif // CHART_H
