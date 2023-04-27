#include "db.h"

/**
 * @brief DB::DB
 */
DB::DB() {
  db = QSqlDatabase::addDatabase("QSQLITE");
  // Pfad zur Datenbank
  QString dbpath = "kronos.db";
  db.setDatabaseName(dbpath);

  try {
    if (db.open()) {
      qDebug() << "Mit Datenbank verbunden!";
    } else {
      throw 505;
    }

  } catch (...) {
    qDebug() << "Keine Datenbankverbindung!";
  }
}

/**
 * @brief DB::getHO
 * @param a
 * @param start
 * @param end
 * @return
 */
int DB::getHO(int a, QDate start, QDate end) {
  int ho = 0;
  QString s = QString::number(a);
  QSqlQuery q;
  //+qDebug() << a << start << end;
  q.prepare("SELECT * FROM Days WHERE typenID LIKE (:s) AND datum BETWEEN "
            "(:start) AND (:end)");
  q.bindValue(":s", s);
  q.bindValue(":start", start);
  q.bindValue(":end", end);
  q.exec();

  while (q.next()) {
    ho = ho + q.value("minuten").toInt();
    //qDebug() << ho;
  }
  return ho;
}

/**
 * @brief DB::getList
 * @param a
 * @param start
 * @param end
 * @return
 */
QVector<int> DB::getList(int a, QDate start, QDate end) {
  // qDebug() << start;
  QString s = QString::number(a);

  QSqlQuery q;
  q.prepare("SELECT * FROM Days WHERE typenID LIKE (:s) AND datum BETWEEN "
            "(:start) AND (:end)");
  q.bindValue(":s", s);
  q.bindValue(":start", start);
  q.bindValue(":end", end);
  q.exec();

  // qDebug () << q.record().value(1);

  QVector<int> l;
  while (q.next()) {
    l.append(q.value("minuten").toInt());
  }
  return l;
}

QVector<int> DB::getZeit(QDate datum) {
  QVector<int> zeit;
  QSqlQuery q;
  q.prepare("SELECT * FROM Days WHERE datum = (:datum)");
  q.bindValue(":datum", datum);
  q.exec();
  // qDebug() << datum.toString("yyyy-MM-dd");
  while (q.next()) {
    zeit.append(q.value("minuten").toInt());
  }
  return zeit;
}

QVector<QString> DB::getTyp(QDate datum) {
  QVector<QString> typ;
  QSqlQuery q;
  q.prepare("SELECT * FROM Days WHERE datum = (:datum)");
  q.bindValue(":datum", datum);
  q.exec();
  // qDebug() << datum.toString("yyyy-MM-dd");
  while (q.next()) {
    QSqlQuery q2;
    q2.prepare("SELECT name FROM Typen WHERE ID = (:typ)");
    q2.bindValue(":typ", q.value("typenID").toInt());
    q2.exec();
    while (q2.next()) {
      typ.append(q2.value("name").toString());
    }
  }
  return typ;
}
