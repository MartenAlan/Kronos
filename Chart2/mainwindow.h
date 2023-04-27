#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chart.h"
#include "db.h"
#include <QComboBox>
#include <QDesktopServices>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTextStream>
#include <QUrl>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void ListModel();
  void tabelle(QDate start, QDate end);

  Ui::MainWindow *getUi() const;
  void setUi(Ui::MainWindow *newUi);
  Chart c;

private slots:
  void on_pushButtonLineChart_clicked();

  void on_pushButtonPieChart_clicked();

  void on_comboBox_currentIndexChanged(int index);

  void on_pushButtonComboBox_clicked();

  void on_pushButton_clicked();

  void on_pushButtonDate_clicked();

private:
  Ui::MainWindow *ui;
  QStandardItemModel *model;
};
#endif // MAINWINDOW_H
