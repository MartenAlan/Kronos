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
  createTables();
  fillTypesTable();
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
    qDebug() << q.value("minuten");
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

void DB::execQueryWithoutResultSet(QString queryString){
QSqlQuery roundsQuery(queryString);
if (!roundsQuery.isActive()) {
    qWarning() << "ERROR: Create Table " << roundsQuery.lastError();
    }
}

void DB::createTables(){

    // NOTE Create Types Table
    QString queryString = "CREATE TABLE `dayResults` ("
            "`ID`       INTEGER,"
            "`soll`     INTEGER,"
            "`ntGes`	INTEGER,"
            "`saldo`	INTEGER,"
            "PRIMARY KEY(`ID` AUTOINCREMENT)"
        ");";
    execQueryWithoutResultSet(queryString);

    // NOTE Create Types Table
    queryString = "CREATE TABLE `types` ("
            "`ID`       INTEGER,"
            "`initials`	TEXT    NOT NULL UNIQUE,"
            "`name`     TEXT ,"
            "PRIMARY KEY(`ID` AUTOINCREMENT)"
        ");";
    execQueryWithoutResultSet(queryString);

    // NOTE Create Journals Table
    queryString = "CREATE TABLE `journals` ("
            "`ID`	INTEGER,"
            "`printdate`	TEXT    NOT NULL UNIQUE,"
            "`datum`         TEXT NOT NULL,"
            "`urlGutAlt`	INTEGER NOT NULL,"
            "`urlGutGes`	INTEGER NOT NULL,"
            "`urlGutAkt`	INTEGER NOT NULL,"
            "`urlGutRst`	INTEGER NOT NULL,"
            "`soll`         INTEGER NOT NULL,"
            "`ntGes`        INTEGER NOT NULL,"
            "`saldo`        INTEGER NOT NULL,"
            "`diffNt`       INTEGER NOT NULL,"
            "PRIMARY KEY(`ID` AUTOINCREMENT)"
        ");";
    execQueryWithoutResultSet(queryString);

    // NOTE Create Error Table
    queryString = "CREATE TABLE `errors` ("
            "`ID`       INTEGER,"
            "`name`     TEXT NOT NULL UNIQUE,"
            "`message`	TEXT NOT NULL,"
            "PRIMARY KEY(`ID` AUTOINCREMENT)"
        ");";
    execQueryWithoutResultSet(queryString);

    // NOTE Create Days Table
    queryString =     "CREATE TABLE `days` ("
                "`ID`           INTEGER,"
                "`typenID`       INTEGER NOT NULL,"
                "`errorID`      INTEGER,"
                "`journalID`	INTEGER NOT NULL,"
                "`dayResultID`	INTEGER NOT NULL,"
                "`datum`         TEXT NOT NULL,"
                "`minuten`      INT NOT NULL,"
                "`minutenRaw`   INT NOT NULL,"
                "FOREIGN KEY(`typenID`) REFERENCES `types`(`ID`),"
                "FOREIGN KEY(`errorID`) REFERENCES `errors`(`ID`),"
                "FOREIGN KEY(`journalID`) REFERENCES `journals`(`ID`),"
                "FOREIGN KEY(`dayResultID`) REFERENCES `dayResults`(`ID`),"
                "PRIMARY KEY(`ID` AUTOINCREMENT)"
            ");";
    execQueryWithoutResultSet(queryString);
}

void DB::fillTypesTable(){
    QSqlQuery q;
     QVector <QString> types = getTypeMap();
     qDebug() << "test";
    for(int i = 0;i<types.length();i++){
        q.prepare("INSERT INTO types (initials, name) "
                  "VALUES (:initials, :name)");
        q.bindValue(":initials", types[i]);
        q.bindValue(":name", "Test");
        qDebug() << q.exec();
        if (!q.isActive()) {
            qWarning() << "ERROR: Create Table " << q.lastError();
            }
    }
    }

void DB::fillErrorTable(){
    QSqlQuery q;
    QMap<QString, QString> map = getErrorMap();
    QMapIterator<QString, QString> i(map);
    while (i.hasNext()) {
        i.next();
        q.prepare("INSERT INTO errors (name, message) "
                  "VALUES (:name, :message)");
        q.bindValue(":initials", i.key());
        q.bindValue(":name", i.value());
        qDebug() << q.exec();
        if (!q.isActive()) {
            qWarning() << "ERROR: Can't Insert Into Errors " << q.lastError();
            }
    }
}


void DB::insertNewDay(Day d){
    QSqlQuery q;
    for (const TimeInterval &x: d.times){
        qDebug() << x.typeID;
    q.prepare("INSERT INTO days (minuten, minutenRaw,"
              " datum, typenID, errorID, journalID, dayResultID) "
                  "VALUES (:minuten, :minutenRaw,"
              " :datum, :typenID, :errorID, :journalID, :dayResultID)");

    q.bindValue(":minuten", x.minutes);
    q.bindValue(":minutenRaw", x.minutesRaw);
    q.bindValue(":datum", d.date);
    q.bindValue(":typenID", x.typeID);
    q.bindValue(":errorID", "NULL");
    q.bindValue(":journalID", d.journalID);
    q.bindValue(":dayResultID", d.dayResultsID);
    q.exec();
    if (!q.isActive()) {
        qWarning() << "ERROR: Can't Insert Into Days " << q.lastError();
        }
    }
}

int DB::insertNewJournal(Journal * j){
    QSqlQuery q;
    q.prepare("INSERT INTO 'journals' (printdate, datum, urlGutAlt, "
              "urlGutAkt,urlGutRst,urlGutGes, soll, ntges, saldo, diffNt) "
                  "VALUES (:printdate, :datum, :urlGutAlt, :urlGutAkt, "
              ":urlGutRst, :urlGutGes, :soll, :ntges, :saldo, :diffNt)");
    q.bindValue(":printdate", j->druckdatum.toString("dd-MM-yyyy hh:mm:ss"));
    q.bindValue(":datum", j->datum.toString("dd-MM-yyyy"));
    q.bindValue(":urlGutAlt", j->urlalt);
    q.bindValue(":urlGutAkt", j->urlakt);
    q.bindValue(":urlGutRst", j->urlrst);
    q.bindValue(":urlGutGes", j->urlges);
    q.bindValue(":soll", j->soll);
    q.bindValue(":ntges", j->ntges);
    q.bindValue(":saldo", j->saldo);
    q.bindValue(":diffNt", j->diffnt);
    q.exec();
    if(!q.isActive()) {
        qWarning() << "ERROR: Can't Insert Into Journals " << q.lastError() << " ";
        return -1;
        }
    else{
    return q.lastInsertId().toInt();
    }
}

int DB::insertNewDayResults(Day d){
    QSqlQuery q;

    q.prepare("INSERT INTO dayResults (soll, ntGes, saldo) "
              "VALUES (:soll, :ntGes, :saldo)");
    q.bindValue(":soll", d.soll);
    q.bindValue(":ntGes", d.ntges);
    q.bindValue(":saldo", d.saldo);
    q.exec();
    if (!q.isActive()) {
        qWarning() << "ERROR: Can't Insert Into DayResults " << q.lastError();
        }
    return q.lastInsertId().toInt();
}

QVector<QString> DB::getTypeMap(){
    QVector<QString> map(7);
    map[0] = "FA";
    map[1] = "O";
    map[2] = "URL";
    map[3] = "KRK";
    map[4]= "GLT";
    map[5] = "ABS";
    map[6] = "WE";

    return map;
}

QMap<QString, QString> DB::getErrorMap(){
    QMap<QString, QString> map;
    map["Fehler"] = "Ein Fehler";
    return map;
}


