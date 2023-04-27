#include "tabelle.h"

tabelle::tabelle()
{

}

void tabelle::ListModel() {
    model = new QStandardItemModel(0,6,this);
    model->setHorizontalHeaderItem(0, new QStandardItem("Land"));
    model->setHorizontalHeaderItem(1, new QStandardItem("PLZ"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Ort"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Region"));
    model->setHorizontalHeaderItem(4, new QStandardItem("Latitude"));
    model->setHorizontalHeaderItem(5, new QStandardItem("Longitude"));

    MainWindow::ui->tableView->setModel(model);
};
