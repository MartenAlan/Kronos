#include "chart.h"

/**
 * @brief Chart::createPieChart
 * @param i
 * @param start
 * @param end
 * @return
 */

QChartView *Chart::createPieChart(int i, QDate start, QDate end) {

  float gesamt = db.getHO(1, start, end) + db.getHO(2, start, end) +
                 db.getHO(3, start, end) + db.getHO(4, start, end) +
                 db.getHO(5, start, end);

  QPieSeries *series = new QPieSeries();
  series->setPieSize(0.6);
  series
      ->append(
          "Urlaub " +
              QString::number(qRound(db.getHO(3, start, end) / gesamt * 100)) +
              "%",
          db.getHO(3, start, end))
      ->setColor(0x68B42E);
  series
      ->append(
          "Home Office " +
              QString::number(qRound(db.getHO(1, start, end) / gesamt * 100)) +
              "%",
          db.getHO(1, start, end))
      ->setColor(0xBBF98B);
  series
      ->append(
          "Krankheit " +
              QString::number(qRound(db.getHO(4, start, end) / gesamt * 100)) +
              "%",
          db.getHO(4, start, end))
      ->setColor(0x90F740);
  series
      ->append(
          "Büro " +
              QString::number(qRound(db.getHO(2, start, end) / gesamt * 100)) +
              "%",
          db.getHO(2, start, end))
      ->setColor(0x5A7843);
  series
      ->append(
          "Gleittage " +
              QString::number(qRound(db.getHO(5, start, end) / gesamt * 100)) +
              "%",
          db.getHO(5, start, end))
      ->setColor(0x72C433);

  QPieSlice *slice = series->slices().at(i);
  slice->setExploded();
  slice->setLabelVisible();
  slice->setPen(QPen(Qt::darkGreen, 2));

  QChart *chart = new QChart();
  chart->addSeries(series);
  chart->size();
  chart->setTitle("Verhältnis");
  chart->legend()->hide();

  QChartView *chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  return chartView;
}

/**
 * @brief Chart::createLineChart
 * @param start
 * @param end
 * @return
 */
QChartView *Chart::createLineChart(QDate start, QDate end) {

  QVector<int> l = db.getList(1, start, end);

  QChart *chart = new QChart();

  QLineSeries *series = new QLineSeries(chart);
  series->setName("Home Office");

  for (int i = 0; i < l.length(); i++) {
    *series << QPointF(i, l[i]);
  }
  chart->addSeries(series);

  l = db.getList(2, start, end);

  QLineSeries *series2 = new QLineSeries(chart);
  series2->setName("Büro");

  for (int i = 0; i < l.length(); i++) {
    *series2 << QPointF(i, l[i]);
  }

  chart->addSeries(series2);

  // Customize series
  QPen pen(QRgb(0x68B42E));
  pen.setWidth(1);
  series->setPen(pen);

  QPen pen2(QRgb(0xb4412e));
  pen2.setWidth(1);
  series->setPen(pen2);

  // Customize chart title
  QFont font;
  font.setPixelSize(18);
  chart->setTitleFont(font);
  chart->setTitleBrush(QBrush(0x5A7843));
  chart->setTitle(start.toString("dd.MM.yy") + " - " +
                  end.toString("dd.MM.yy"));

  QCategoryAxis *axisX = new QCategoryAxis();
  QCategoryAxis *axisY = new QCategoryAxis();

  // Customize axis label font
  QFont labelsFont;
  labelsFont.setPixelSize(12);
  axisX->setLabelsFont(labelsFont);
  axisY->setLabelsFont(labelsFont);

  // Customize axis colors
  QPen axisPen(QRgb(0x5A7843));
  axisPen.setWidth(2);
  axisX->setLinePen(axisPen);
  axisY->setLinePen(axisPen);

  // Customize axis label colors
  QBrush axisBrush(0x68B42E);
  axisX->setLabelsBrush(axisBrush);
  axisY->setLabelsBrush(axisBrush);

  // Customize grid lines and shades
  axisX->setGridLineVisible(false);
  axisY->setGridLineVisible(true);
  axisY->setShadesPen(Qt::NoPen);
  axisY->setShadesBrush(Qt::NoBrush);
  axisY->setShadesVisible(true);

  int diff = start.daysTo(end);

  for (int i = 0; i < (diff + 1); i++) {
    axisX->append(start.addDays(i).toString("d"), i + 1);
  }
  axisX->append(end.toString("d"), diff + 2);

  axisX->setRange(0, start.daysTo(end) + 1);

  axisY->append("468", 468);

  axisY->setRange(0, 735);

  chart->addAxis(axisX, Qt::AlignBottom);
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisX);
  series->attachAxis(axisY);
  series2->attachAxis(axisX);
  series2->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  return chartView;
}
/**
 * @brief Chart::Chart_HO_non_static
 * @param start
 * @param end
 * @return
 */
QChartView *Chart::Chart_HO_non_static(QDate start, QDate end) {

  float gesamt = db.getHO(1, start, end) + db.getHO(2, start, end);

  QPieSeries *series = new QPieSeries();
  series->setPieSize(0.7);
  series->append("Home Office", db.getHO(1, start, end))->setColor(0xBBF98B);
  series->append("Büro", db.getHO(2, start, end))->setColor(0x5A7843);
  series->slices().at(0)->setLabelVisible();
  series->slices().at(1)->setLabelVisible();

  QChart *chart = new QChart();
  chart->addSeries(series);
  chart->size();
  chart->setTitle(
      "Büro " +
      QString::number(qRound(db.getHO(2, start, end) / gesamt * 100)) +
      "% / Home Office " +
      QString::number(qRound(db.getHO(1, start, end) / gesamt * 100)) + "%");
  chart->legend()->hide();

  QChartView *chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  return chartView;
}

QVector<int> Chart::TabelleMinuten(QDate datum) {
  QVector<int> zeit;
  zeit = db.getZeit(datum);
  return zeit;
}

QVector<QString> Chart::TabelleTyp(QDate datum) {
  QVector<QString> typ;
  typ = db.getTyp(datum);
  return typ;
}
