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
unsigned long intervalo_loop=1000;
unsigned long previoMillisLoop=0;
unsigned long previoMillisTermo=0;
unsigned long previoMillisAUTO=0;
unsigned long actualMillis;
byte horaON1;
byte horaOFF1;
byte horaON2;
byte horaOFF2;
byte posON1;
byte posON2;
byte posOFF1;
byte posOFF2;
byte posTEMP;
byte cont;
bool estado_termo;
bool espera;
bool espera_man;
bool start;
byte manual;
byte estado;
byte hr, mn, sg, di, me;
int an;

void setup() {
    Serial.begin(115200);
    myservo_X.attach(9);
    reloj.begin();
    estado=1;
    estado_termo=0;
    start=1;
}

void loop() {
    if(start) {
        myservo_X.write(30);
        fecha=reloj.now();
        cargarVarsControl();
        start=0;
    }
    actualMillis = millis();
    if ((unsigned long)(actualMillis - previoMillisLoop) >= intervalo_loop) {
        printVars();
        fecha = reloj.now();
        switch(estado) {
            case 1:
                // funcionamiento autonomo, por defecto
                if ((unsigned long)(actualMillis - previoMillisAUTO) >= 1000) {
                    if((((fecha.hour() >= horaON1)   && (fecha.hour() < horaOFF1)) ||
                        ((fecha.hour() >= horaON2) && (fecha.hour() < horaOFF2))) && (estado_termo==0)) {
                        termoACC(posON1, posON2);
                    } else if((((fecha.hour() < horaON1)   || (fecha.hour() >= horaOFF1)) && 
                               ((fecha.hour() < horaON2) || (fecha.hour() >= horaOFF2))) && (estado_termo==1)) {
                        termoACC(posOFF1, posOFF2);
                    }
                }
                previoMillisAUTO = millis();
                break;
            case 2:
                // ajustaReloj
                setVarsEP();
                estado = 1;
                break;
            case 3:
                // ajuste horas de funcionamiento
                setVarsEP();
                cargarVarsControl();
                estado = 1;
                break;
            case 4:
                // ajustaPosServo
                setVarsEP();
                cargarVarsControl();
                estado = 1;
                break;
            case 5:
                // ON y OFF manual
                if(espera_man) {
                    if(manual == 1) {
                        termoACC(posON1, posON2);
                    } else if (manual == 0){
                        termoACC(posOFF1, posOFF2);
                    }
                    espera_man=0;
                }
                break;
            case 6:
                // movimiento 'libre'
                if(validaPOS(posTEMP)) {
                    myservo_X.write(posTEMP);
                    estado_termo=0;
                }
                break;
            default:
                estado=1;
        }
        leerDatos();
        previoMillisLoop = millis();
    }
}

void leerDatos() {
    if (Serial.available() > 0) {
        byte temp_var=0;
        temp_var = Serial.readStringUntil(',').toInt();
        if(temp_var < 7 && temp_var > 0){
            estado = temp_var;
        } else {
            estado = 1;
        }
        switch (estado) {
            case 1:
                limpiaSerial(); 
                break;
            case 2:
                di = Serial.readStringUntil(',').toInt();
                me = Serial.readStringUntil(',').toInt();
                an = Serial.readStringUntil(',').toInt();
                hr = Serial.readStringUntil(',').toInt();
                mn = Serial.readStringUntil(',').toInt();
                sg = Serial.readStringUntil(',').toInt();
                break;
            case 3:
                horaON1 = Serial.readStringUntil(',').toInt();
                horaOFF1 = Serial.readStringUntil(',').toInt();
                horaON2 = Serial.readStringUntil(',').toInt();
                horaOFF2 = Serial.readStringUntil(',').toInt();
                break;
            case 4:
                posON1 = Serial.readStringUntil(',').toInt();
                posON2 = Serial.readStringUntil(',').toInt();
                posOFF1 = Serial.readStringUntil(',').toInt();
                posOFF2 = Serial.readStringUntil(',').toInt();
                break;
            case 5:
                manual = Serial.readStringUntil(',').toInt();
                espera_man = 1;
                break;
            case 6:
                posTEMP = Serial.readStringUntil(',').toInt();
                break;
            default:
                limpiaSerial();
        }
        limpiaSerial();
    }
}



void limpiaSerial() {
    while(Serial.available() > 0) {
        char temp = Serial.read();
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
                espera=1;
                cont++;
                previoMillisTermo=millis();
            }
        }
        actualMillis=millis();
        if(espera==1) {
            if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
                myservo_X.write(pos1);
                espera=0;
                cont++;
                previoMillisTermo=millis();
            }
        }
    } while (cont<4);
    estado_termo=!estado_termo;
}

void setVarsEP(){
    switch (estado) {
        case 2:
            ajustaReloj(an, me, di, hr, mn, sg);
            break;
        case 3:
            if(validaHR(horaON1)) {
                EEPROM.put(dirHoraON1, horaON1);
            } else {
                horaON1 = EEPROM.read(dirHoraON1);
            }
            if(validaHR(horaOFF1) && horaOFF1 > horaON1) {
                EEPROM.put(dirHoraOFF1, horaOFF1);
            } else {
                EEPROM.put(dirHoraOFF1, horaON1 + 1);
                horaOFF1 = EEPROM.read(dirHoraOFF1);
            }
            if(validaHR(horaON2) && horaON2 > horaOFF1) {
                EEPROM.put(dirHoraON2, horaON2);
            } else if(validaHR(horaON1)) {
                EEPROM.put(dirHoraON2, horaON1);
                EEPROM.put(dirHoraOFF2, horaOFF1);
                horaON2 = EEPROM.read(dirHoraON2);
                horaOFF2 = EEPROM.read(dirHoraOFF2);
            }
            if(validaHR(horaOFF2) && (horaOFF2 > horaON2) && (horaOFF2 > horaOFF1)) {
                EEPROM.put(dirHoraOFF2, horaOFF2);
            } else {
                EEPROM.put(dirHoraON2, horaON1);
                EEPROM.put(dirHoraOFF2, horaOFF1);
            }
            break;
        case 4:
            if(validaPOS(posON1)) {
                EEPROM.put(dirPosON1, posON1);
            }
            if(validaPOS(posON2)) {
                EEPROM.put(dirPosON2, posON2);
            }
            if(validaPOS(posOFF1)) {
                EEPROM.put(dirPosOFF1, posOFF1);
            }
            if(validaPOS(posOFF2)) {
                EEPROM.put(dirPosOFF2, posOFF2);
            }
            break;
    }
}

bool validaHR(byte hr) {
    return (hr < 24 && hr >= 0);
}

bool validaPOS(byte pos) {
    return (pos < 165) && (pos >15);
}

void cargarVarsControl() {
    horaON1 = EEPROM.read(dirHoraON1);
    horaOFF1 = EEPROM.read(dirHoraOFF1);
    horaON2 = EEPROM.read(dirHoraON2);
    horaOFF2 = EEPROM.read(dirHoraOFF2);
    posON1 = EEPROM.read(dirPosON1);
    posON2 = EEPROM.read(dirPosON2);
    posOFF1 = EEPROM.read(dirPosOFF1);
    posOFF2 = EEPROM.read(dirPosOFF2);
}

void ajustaReloj(int ano, byte mes, byte dia, byte hra, byte mins, byte segs ) {
    if((ano > 2000) && 
       (mes > 0 && mes < 13) && 
       (dia > 0 && dia < 32) && 
       (hra >= 0 && hra < 24) && 
       (mins >= 0 && mins < 60) && 
       (segs >= 0 && segs < 60)) {
        DateTime dt(ano, mes, dia, hra, mins, segs);
        reloj.adjust(dt);
    }
}

void printVars() {
    Serial.println((String)estado+","+(String)estado_termo+","+(String)horaON1+","+(String)horaOFF1+","+(String)horaON2+","+
                   (String)horaOFF2+","+(String)posON1+","+(String)posON2+","+(String)posOFF1+","+(String)posOFF2+","+
                   (String)fecha.hour()+","+(String)fecha.minute()+","+(String)fecha.second()+","+(String)fecha.day()+","+
                   (String)fecha.month()+","+(String)fecha.year());
}
