#include <Servo.h>
#include <RTClib.h>
#include <EEPROM.h>

#define dirHoraON1 0
#define dirHoraOFF1 1
#define dirHoraON2 2
#define dirHoraOFF2 3
#define dirPosON1 4
#define dirPosON2 5
#define dirPosOFF1 6
#define dirPosOFF2 7

Servo myservo_X;
RTC_DS3231 reloj;

volatile DateTime fecha;
unsigned long intervalo_loop=5000;
unsigned long previoMillisTermo=0;
unsigned long previoMillisLoop=0;
unsigned long actualMillis;
byte horaON1;
byte horaOFF1;
byte horaON2;
byte horaOFF2;
byte posON1;
byte posON2;
byte posOFF1;
byte posOFF2;
byte cont;
bool estado_termo;
bool espera;
bool start;

void setup() {
    Serial.begin(9600);
    myservo_X.attach(9);
    reloj.begin();
    //setVarControl(); // Provisorio, a dividir segun control horario, posiciones. futuras vars
    estado_termo=0;
    setVarControl();
    variablesControl();
    start=1;
    ajustaReloj(2021, 7, 8, 5, 30, 0);
    //reloj.adjust(DateTime(__DATE__, __TIME__));
}

void loop() {
    if(start) {
        myservo_X.write(30);
        start=0;
        fecha=reloj.now();
    }
    actualMillis = millis();
    if ((unsigned long)(actualMillis - previoMillisLoop) >= intervalo_loop) {
        printFecha();
        fecha = reloj.now();
        if((((fecha.hour() >= horaON1)   && (fecha.hour() < horaOFF1)) ||
            ((fecha.hour() >= horaON2) && (fecha.hour() < horaOFF2))) && (estado_termo==0)) {
            termoACC(posON1, posON2);
            Serial.println("TERMO ON");
        } else if((((fecha.hour() < horaON1)   || (fecha.hour() >= horaOFF1)) && 
                   ((fecha.hour() < horaON2) || (fecha.hour() >= horaOFF2))) && (estado_termo==1)) {
            termoACC(posOFF1, posOFF2);
            Serial.println("TERMO OFF");
        }
        previoMillisLoop = millis();
    }
}

void termoACC(byte pos1, byte pos2) {
    myservo_X.write(pos1);
    espera=0;
    cont=0;
    previoMillisTermo=millis();
    do{
        actualMillis=millis();
        if(espera==0) {
            if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
                myservo_X.write(pos2);
                previoMillisTermo=millis();
                espera=1;
                cont++;
            }
        }
        if(espera==1) {
            if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
                myservo_X.write(pos1);
                previoMillisTermo=millis();
                espera=0;
                cont++;
            }
        }
    } while (cont<4);
    estado_termo=!estado_termo;
}

// provisorio, case config para hora, horas o dias, posiciones, etc.
void setVarControl() {
    //horaON1 = 5;
    //horaOFF1 = 7;
    //horaON2 = 16;
    //horaOFF2 = 18;
    //posON1 = 120;
    //posON2 = 110;
    //posOFF1 = 45;
    //posOFF2 = 62;   
    //EEPROM.put(dirHoraON1, horaON1);
    //EEPROM.put(dirHoraOFF1, horaOFF1);
    //EEPROM.put(dirHoraON2, horaON2);
    //EEPROM.put(dirHoraOFF2, horaOFF2);
    //EEPROM.put(dirPosON1,posON1);
    //EEPROM.put(dirPosON2, posON2);
    //EEPROM.put(dirPosOFF1, posOFF1); 
    //EEPROM.put(dirPosOFF2, posOFF2);
}

void variablesControl() {
    horaON1 = EEPROM.read(dirHoraON1);
    horaOFF1 = EEPROM.read(dirHoraOFF1);
    horaON2 = EEPROM.read(dirHoraON2);
    horaOFF2 = EEPROM.read(dirHoraOFF2);
    posON1 = EEPROM.read(dirPosON1);
    posON2 = EEPROM.read(dirPosON2);
    posOFF1 = EEPROM.read(dirPosOFF1);
    posOFF2 = EEPROM.read(dirPosOFF2);
    Serial.println("Variables: ");
    Serial.println((String)horaON1);
    Serial.println((String)horaOFF1);
    Serial.println((String)horaON2);
    Serial.println((String)horaOFF2);
    Serial.println((String)posON1);
    Serial.println((String)posON2);
    Serial.println((String)posOFF1);
    Serial.println((String)posOFF2);

}

void ajustaReloj(int ano, byte mes, byte dia, byte hra, byte mins, byte segs ) {
    DateTime dt(ano, mes, dia, hra, mins, segs);
    reloj.adjust(dt);
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
