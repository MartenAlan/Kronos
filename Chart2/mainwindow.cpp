#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->dateEditStart->setDate(QDate::currentDate());
  ui->dateEditEnd->setDate(QDate::currentDate());
  ui->pushButton->setStyleSheet("background-color: transparent");
  qDebug() << ui->dateEditStart->date().toString("yyyy-MM-dd");
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow() { delete ui; }

void MainWindow::ListModel() {
  model = new QStandardItemModel(0, 3, this);
  model->setHorizontalHeaderItem(0, new QStandardItem("Datum"));
  model->setHorizontalHeaderItem(1, new QStandardItem("Arbeitszeit"));
  model->setHorizontalHeaderItem(2, new QStandardItem("Typ"));

  ui->tableView->setModel(model);
};

/**
 * @brief MainWindow::tabelle
 * @param start
 * @param end+ MainWindo
 */

void MainWindow::tabelle(QDate start, QDate end) {
  ListModel();
  QVector<int> zeit;
  QVector<QString> typ;
  int i = 0;
  int j = 0;
  QString datum;
  QString minuten;
  QString typen;

  while (start.addDays(i) <= end) {
    zeit = c.TabelleMinuten(start.addDays(i));
    typ = c.TabelleTyp(start.addDays(i));

    for (int k = 0; k < zeit.length(); k++) {
      datum = start.addDays(i).toString("dd.MM.yy");
      QStandardItem *colDatum = new QStandardItem();
      colDatum->setText(datum);
      model->setItem(j, 0, colDatum);

      minuten.setNum(zeit[k]);
      QStandardItem *colMin = new QStandardItem();
      colMin->setText(minuten);
      model->setItem(j, 1, colMin);

      typen = typ[k];
      QStandardItem *colTyp = new QStandardItem();
      colTyp->setText(typen);
      model->setItem(j, 2, colTyp);

      j++;
    }
    i++;
  }
}

/**
 * @brief MainWindow::on_pushButtonLineChart_clicked
 */
void MainWindow::on_pushButtonLineChart_clicked() {
  delete ui->frameCharts->layout();
  QVBoxLayout *layout = new QVBoxLayout(ui->frameCharts);
  layout->addWidget(
      c.createLineChart(ui->dateEditStart->date(), ui->dateEditEnd->date()));
  MainWindow::tabelle(ui->dateEditStart->date(), ui->dateEditEnd->date());
}

/**
 * @brief MainWindow::on_pushButtonPieChart_clicked
 */
void MainWindow::on_pushButtonPieChart_clicked() {
  delete ui->frameCharts->layout();
  QVBoxLayout *layout = new QVBoxLayout(ui->frameCharts);
  layout->addWidget(c.Chart_HO_non_static(ui->dateEditStart->date(),
                                          ui->dateEditEnd->date()));
}

/**
 * @brief MainWindow::on_comboBox_currentIndexChanged
 * @param index
 */
void MainWindow::on_comboBox_currentIndexChanged(int index) {
  delete ui->frameCharts->layout();
  QVBoxLayout *layout = new QVBoxLayout(ui->frameCharts);
  layout->addWidget(c.createPieChart(index, ui->dateEditStart->date(),
                                     ui->dateEditEnd->date()));
}

/**
 * @brief MainWindow::on_pushButtonComboBox_clicked
 */
void MainWindow::on_pushButtonComboBox_clicked() {
  delete ui->frameCharts->layout();
  QVBoxLayout *layout = new QVBoxLayout(ui->frameCharts);
  layout->addWidget(c.createPieChart(ui->comboBox->currentIndex(),
                                     ui->dateEditStart->date(),
                                     ui->dateEditEnd->date()));
}

/**
 * @brief MainWindow::on_pushButton_clicked
 */
void MainWindow::on_pushButton_clicked() {
  QDesktopServices::openUrl(QUrl("https://svlfg.de/"));
}

/**
 * @brief MainWindow::on_pushButtonDate_clicked
 */
void MainWindow::on_pushButtonDate_clicked() {
  delete ui->frameCharts->layout();
  QVBoxLayout *layout = new QVBoxLayout(ui->frameCharts);
  layout->addWidget(
      c.createLineChart(ui->dateEditStart->date(), ui->dateEditEnd->date()));
  MainWindow::tabelle(ui->dateEditStart->date(), ui->dateEditEnd->date());
}
