#include "day.h"

Day::Day()
{
    saldo = 0, ntges = 0, soll = 0;
}


TimeInterval::TimeInterval(QString type){
    this->type = type;
    minutesRaw = 0;
    minutes = 0;
}

TimeInterval::TimeInterval(QTime begin, QTime end){
    this->begin = begin;
    this->end = end;
    this->minutesRaw = begin.secsTo(end) / 60;
    this->minutes = begin.secsTo(end) / 60;
}

TimeInterval::TimeInterval(QTime begin){
    this->begin = begin;
    minutesRaw=0;
    minutes=0;
}

void TimeInterval::getMinutesRaw(){
    this->minutesRaw = begin.secsTo(end) / 60;
    this->minutes = begin.secsTo(end) / 60;
}


void Day::calculateMinutes(){

    // FIXME 9 Stunden Pausenzeit noch nicht berücksichtigt, vieles andere wahrscheinlich auch noch nicht

    // bei O von hinterer Zeit richtige Länge abziehen. einmal bei 8 und einmal bei 9 Stunden
    int minutes = 0;int minutesRaw = 0;
    int pause = 0;

    if(times.length() > 1){ // mehr als eine Buchung am Tag
   for(int i=0;i<times.length()-1;i++){
       pause += times.at(i).end.secsTo(times.at(i+1).begin) / 60;
   }
    }
   for(auto x: times){
        minutesRaw += x.minutesRaw;
        x.minutes = x.minutesRaw;
        qDebug() << x.minutesRaw;
   }

    qDebug() << "Pause: " << pause;
    if(pause < 30 and minutesRaw > 6*60){
       times.last().minutes = times.last().minutesRaw - (30 - pause);
       qDebug() << "Neu: " << times.last().minutes << "<--" <<  times.last().minutesRaw;
    }
     qDebug() << date << " | " << minutes << ", " << ntges;

     // andere Zeiten fehlen
    for(const auto &y: times){
        qDebug() << y.minutes;
         minutes += y.minutes;
    }

    qDebug() << "Neu Ges: " << minutes;


   if(minutes == ntges){
       qDebug() << "Passt";
   }
   else{
       qDebug() << "Passt nicht";
   }
   qDebug() << "";

}
