#include "journal.h"

Journal::Journal()
{
    indexes["TP"] = 3;

}

void Journal::setIndexesTo33(){
    indexes["Druckdatum"] = 5;
    indexes["time1"] = 12;
    indexes["time2"] = 15;
    indexes["Bemerkung"] = 22;
    indexes["fz"] = 20;
    indexes["Soll"] = 27;
    indexes["NtGes"] = 30;
    indexes["SollMon"] = 21;
    indexes["NtGesMon"] = 23;

}

void Journal::setIndexesTo36(){
    indexes["Druckdatum"] = 6;
    indexes["time1"]= 13;
    indexes["time2"]= 16;
    indexes["Bemerkung"] = 24;
    indexes["fz"] = 22;
    indexes["Soll"] = 30;
    indexes["NtGes"] = 33;
    indexes["SollMon"] = 23;
    indexes["NtGesMon"] = 25;
}


// NOTE Herausfiltern der Metainformationen ( Zeile 0-6 )
void Journal::getMetaInfos(){
    // Datum herausfiltern
    datum = QDateTime::fromString(getCell(5,0),"dd.MM.yyyy hh:mm:ss").date();
    // Druckdatum herausfiltern
    druckdatum = QDateTime::fromString(getCell(2, indexes["Druckdatum"]),"dd.MM.yyyy hh:mm:ss");
}

// NOTE Herausfiltern der Tagesinformationen
void Journal::getDayInfos(){
    // 7 = Anfang von Tagesinformationen
    for ( int i = 7; i<journaltext.length(); i++){
        // Sind keine Tage mehr vorhanden?
        if(getCell(i,1) == ""){
            // Ja
            break;
        }
        // Sind Datum 1 und Datum 2 identisch?
        else if(days.isEmpty() or days.last().date != QDateTime::fromString(getCell(i,1),"dd.MM.yyyy hh:mm:ss").date()){
            // Ja --> Neuer Tag wird erstellt
            days.append(Day());
            days.last().date = QDateTime::fromString(getCell(i,1),"dd.MM.yyyy hh:mm:ss").date();

            // Handelt es sich um einen Wochenende-Tag?
            if(getCell(i,3) == "101"){
                days.last().times.append(TimeInterval("WE oder KA"));
                continue;
            }
        }

            // zwei Zeiten?
            if(getCell(i, indexes["time1"])!="" and getCell(i, indexes["time2"])!="" and getCell(i, indexes["Bemerkung"]) != "[40] flexibles Arbeiten"){
                QTime begin = QTime::fromString(getCell(i, indexes["time1"]));
                QTime end = QTime::fromString(getCell(i, indexes["time2"]));
                days.last().times.append(TimeInterval(begin, end));
                days.last().times.last().type = "O";


            }

            // eine Zeit? ( also FA )
            else if(getCell(i, indexes["time2"])!="" and getCell(i, indexes["time1"])==""){

                 if(getCell(i,indexes["fz"])=="FA"){
                     days.last().times.append(TimeInterval(QTime::fromString(getCell(i, indexes["time2"]))));
                     days.last().times.last().type = "FA";
                 }
                 else{
                     days.last().times.last().end = QTime::fromString(getCell(i, indexes["time2"]));
                     days.last().times.last().getMinutesRaw();
                 }

            }

            // nach Abkürzungen kontrollieren
            if(getType(getCell(i,indexes["fz"])) != ""){
                days.last().times.append(getType(getCell(i,indexes["fz"])));
            }

            // Tagesdaten hinzufügen,
            if(getCell(i, indexes["Soll"]) != ""){
                days.last().soll = getCell(i, indexes["Soll"]).replace(",", ".").toFloat();
                days.last().ntges = getCell(i, indexes["NtGes"]).replace(",", ".").toFloat();
                days.last().saldo = getCell(i, indexes["NtGes"]+1).replace(",", ".").toFloat(); // Saldo
            }
}
}


void Journal::getMonthInfos(){
    for(int i = 7; i<journaltext.length(); i++){
        if(getCell(i,indexes["SollMon"]) != ""){
              soll = getCell(i+1,indexes["SollMon"]).replace(",", ".").toFloat();
              ntges = getCell(i+1,indexes["NtGesMon"]).replace(",", ".").toFloat();
              diffnt = getCell(i+1,indexes["NtGesMon"]+1).replace(",", ".").toFloat(); // DiffNt
              saldo = getCell(i+1,indexes["NtGesMon"]+2).replace(",", ".").toFloat(); // Saldo
              break;
        }
    }
}

QString Journal::getCell(int row, int column){
    QString cellContent = journaltext.at(row).at(column);
    return cellContent;
}

QString Journal::getType(QString fz){
    if(fz == "KmA" or fz=="KoA"){
        return "KRK";
    }
    else if(fz == "URL"){
        return "URL";
    }
    else if(fz == "ABS"){
        return "ABS";
    }
return "";
}

