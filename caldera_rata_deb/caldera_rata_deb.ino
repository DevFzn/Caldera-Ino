#include <Servo.h>
#include <RTClib.h>

Servo myservo_X;
RTC_DS3231 reloj;

volatile DateTime fecha;
unsigned long intervalo_loop=5000;
unsigned long previoMillisTermo=0;
unsigned long previoMillisLoop=0;
unsigned long actualMillis;
byte horaON=5;
byte horaOFF=7;
byte horaON2=16;
byte horaOFF2=18;
byte cont=0;
bool estado_termo;
bool espera;
bool start;

void setup() {
    Serial.begin(9600);
    myservo_X.attach(9);
    reloj.begin();
    start=1;
    estado_termo=0;
    DateTime dt(2021, 7, 5, 5, 32, 0);
    reloj.adjust(dt);
}

//reloj.adjust(DateTime(__DATE__, __TIME__));

void loop() {
    if(start) {
        Serial.println("Inicio");
        myservo_X.write(30);
        start=0;
        fecha=reloj.now();
    }
    actualMillis = millis();
    if ((unsigned long)(actualMillis - previoMillisLoop) >= intervalo_loop) {
        fecha = reloj.now();
        if((((fecha.hour() >= horaON)   && (fecha.hour() < horaOFF)) ||
            ((fecha.hour() >= horaON2) && (fecha.hour() < horaOFF2))) && (estado_termo==0)) {
            Serial.print("pre ON    : ");
            Serial.println((String)estado_termo);
            termoON();
            Serial.print("post ON   : ");
            Serial.println((String)estado_termo);
        } else if((((fecha.hour() < horaON)   || (fecha.hour() >= horaOFF)) && 
                      ((fecha.hour() < horaON2) || (fecha.hour() >= horaOFF2))) && (estado_termo==1)) {
            Serial.print("pre OFF   : ");
            Serial.println((String)estado_termo);
            termoOFF();
            Serial.print("post OFF  : ");
            Serial.println((String)estado_termo);
        }
        previoMillisLoop = millis();
    }
}
        
// Secuencias de encendido y apagado de Caldera
void termoON() {
    myservo_X.write(120);
    espera=0;
    cont=0;
    previoMillisTermo=millis();
    do{
        actualMillis=millis();
        if(espera==0) {
            if((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
                myservo_X.write(110);
                previoMillisTermo=millis();
                espera=1;
                cont++;
            }
        }
        if(espera==1) {
            if((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
                myservo_X.write(120);
                previoMillisTermo=millis();
                espera=0;
                cont++;
            }
        }
    } while(cont<4);
    estado_termo=1;
}

void termoOFF() {
    myservo_X.write(45);
    espera=0;
    cont=0;
    previoMillisTermo=millis();
    do{
        actualMillis=millis();
        if(espera==0) {
            if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
                myservo_X.write(62);
                previoMillisTermo=millis();
                espera=1;
                cont++;
            }
        }
        if(espera==1) {
            if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
                myservo_X.write(45);
                previoMillisTermo=millis();
                espera=0;
                cont++;
            }
        }
    } while (cont<4);
    estado_termo=0;
}


//En caso de ser necesario mostrar hora, este codigo añade un 0 a los valores menores a 10
String getHora(char var) {
  switch (var) {
    case 'h':
      if(fecha.hour() < 10) {
        return ('0'+(String)fecha.hour()); break;
        } else {
          return (String)fecha.hour();break;
        }
    case 'm':
      if(fecha.minute() <10) {
        return ('0'+(String)fecha.minute()); break;
        } else {
          return (String)fecha.minute();break;
      }
    case 's':
      if(fecha.second() <10) {
        return ('0'+(String)fecha.second()); break;
        } else {
          return (String)fecha.second();break;
      }
    case 'd':
      if(fecha.day() <10) {
        return ('0'+(String)fecha.day()); break;
        } else {
          return (String)fecha.day();break;
      }
    case 'n':
      if(fecha.month() <10) {
        return ('0'+(String)fecha.month()); break;
        } else {
          return (String)fecha.month();break;
      }
    case 'a':
      if(fecha.year() <10) {
        return ('0'+(String)fecha.year()); break;
        } else {
          return (String)fecha.year();break;
      }
  }
}

// Enviar hora y fecha por puerto serie
void printFecha() {
  Serial.print(getHora('d'));
  Serial.print("/");
  Serial.print(getHora('n'));
  Serial.print("/");
  Serial.print(getHora('a'));
  Serial.print("  ");
  Serial.print(getHora('h'));
  Serial.print(":");
  Serial.print(getHora('m'));
  Serial.print(":");
  Serial.println(getHora('s'));
  Serial.println("Día de la semana: ");
  Serial.println((String)fecha.dayOfTheWeek());
}
