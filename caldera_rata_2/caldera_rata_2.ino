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
    myservo_X.attach(9);
    reloj.begin();
    start=1;
    estado_termo=0;
}
//reloj.adjust(DateTime(__DATE__, __TIME__));

void loop() {
    if(start) {
        myservo_X.write(30);
        start=0;
        fecha=reloj.now();
    }
    actualMillis = millis();
    if ((unsigned long)(actualMillis - previoMillisLoop) >= intervalo_loop) {
        fecha = reloj.now();
        if((((fecha.hour() >= horaON)   && (fecha.hour() < horaOFF)) ||
            ((fecha.hour() >= horaON2) && (fecha.hour() < horaOFF2))) && (estado_termo==0)) {
            termoON();
        } else if((((fecha.hour() < horaON)   || (fecha.hour() >= horaOFF)) && 
                      ((fecha.hour() < horaON2) || (fecha.hour() >= horaOFF2))) && (estado_termo==1)) {
            termoOFF();
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
