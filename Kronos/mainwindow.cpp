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

void MainWindow::openFile(){
    qDebug() << "Import File";

    // NOTE Open Text File
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Öffnen eines Journals"), "C:/Users/marte/documents", tr("Journal (*.csv)"));
    processJournal(fileName);

}

void MainWindow::processJournal(QString filename){
    Journal * newJournal = parseFile(filename);
    // insert newjournal into database - Funktion
    // Journal insert
    int journalID = db.insertNewJournal(newJournal);
    if(journalID != -1){
    int dayResultsID = 0;
    // Schleife Tage
    for(auto &x : newJournal->days){
        // DayResults Insert
        dayResultsID = db.insertNewDayResults(x);
        x.dayResultsID = dayResultsID;
        x.journalID = journalID;

        // Day Insert
        db.insertNewDay(x);
        }
    }
    else{
        qDebug() << "Journal wurde bereits importiert.";
    }
}

Journal * MainWindow::parseFile(QString fileName){

    // Einlesen der Datei ins Journal als String Array
    if (fileName.isEmpty())
        return nullptr;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return nullptr;
    }
    QTextStream in(&file);
    QString line;

    // Objekt erstellen
    Journal * newJournal = new Journal();

    int firstRow = 0; int columns = 0;
    QStringList tempStringList;
    while(!in.atEnd()){
        line += in.readLine(); // +=, um newlines richtig zusammenzufügen
        tempStringList = line.split(";");

    // erste Zeile, Spaltenlänge kontrollieren
    if (firstRow == 0)
    {
        columns = tempStringList.length();
        firstRow += 1;
    }
    // danach Schleife bis Schleifenlänge erreicht ist, um newlines zu umgehen
        if(tempStringList.length() >= columns){
            newJournal->journaltext.append(tempStringList);
             tempStringList.clear();
             line = "";
    }
}
    // Objekt anhand Spaltenlänge anpassen
    if(columns == 33){
        newJournal->setIndexesTo33();
    }
    else if (columns == 36){
        newJournal->setIndexesTo36();
    }
    // MetaInfos mit GetCell herausholen
    newJournal->getMetaInfos();

    // Tagesdaten mit GetCell herausholen
    newJournal->getDayInfos();

    // Monatsdaten mit GetCell herausholen
    // TODO noch nicht die Urlaubswerte
    newJournal->getMonthInfos();


    // Ausgabe zur Überprüfung
    qDebug() << "";
    qDebug() << newJournal->datum.toString("MM yyyy")+ " | " + newJournal->druckdatum.toString("dd.MM.yyyy");
    qDebug() << newJournal->ntges << " | " <<  newJournal->soll << " | " << newJournal->saldo << " | " << newJournal->diffnt;
    qDebug() << "";
    for(const auto &x : newJournal->days)
    {
        qDebug() << x.date << "|" << x.saldo << "|" << x.soll << "|" << x.ntges;
        for(const auto &y : x.times){
            qDebug() << y.minutesRaw << " " << y.minutes << " " <<  y.type;
        }
    }

    // TODO rawMinutes --> Minutes, Tage durchgehen, Pausen hinzufügen
    // Anfang ist gemacht, 9 Atunden müssen noch berücksichtigt werden

    for(auto &x : newJournal->days){
        x.calculateMinutes();
    }

    return newJournal;
}
