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
byte cont=0;
bool estado_termo;
bool espera;
bool start;

void setup() {
    myservo_X.attach(9);
    reloj.begin();
    setVarControl(); // Provisorio, a dividir segun control horario, posiciones. futuras vars
    estado_termo=0;
    variablesControl();
    start=1;
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
        fecha = reloj.now();
        if((((fecha.hour() >= horaON1)   && (fecha.hour() < horaOFF1)) ||
            ((fecha.hour() >= horaON2) && (fecha.hour() < horaOFF2))) && (estado_termo==0)) {
            termoACC(posON1, posON2);
        } else if((((fecha.hour() < horaON1)   || (fecha.hour() >= horaOFF1)) && 
                   ((fecha.hour() < horaON2) || (fecha.hour() >= horaOFF2))) && (estado_termo==1)) {
            termoACC(posOFF1, posOFF2);
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
    horaON1 = 5;
    horaOFF2 = 7;
    horaON2 = 16;
    horaOFF2 = 18;
    posON1 = 120;
    posON2 = 110;
    posOFF1 = 45;
    posOFF2 = 62;   
    EEPROM.put(dirHoraON1, horaON1);
    EEPROM.put(dirHoraOFF1, horaOFF1);
    EEPROM.put(dirHoraON2, horaON2);
    EEPROM.put(dirHoraOFF2, horaOFF2);
    EEPROM.put(dirPosON1,posON1);
    EEPROM.put(dirPosON2, posON2);
    EEPROM.put(dirPosOFF1, posOFF1); 
    EEPROM.put(dirPosOFF2, posOFF2);
}

void variablesControl() {
    horaON1 = EEPROM.read(dirHoraON1);
    horaOFF1 = EEPROM.read(dirHoraOFF2);
    horaON2 = EEPROM.read(dirHoraON2);
    horaOFF2 = EEPROM.read(dirHoraOFF2);
    posON1 = EEPROM.read(dirPosON1);
    posON2 = EEPROM.read(dirPosON2);
    posOFF1 = EEPROM.read(dirPosOFF1);
    posOFF2 = EEPROM.read(dirPosOFF2);
}
