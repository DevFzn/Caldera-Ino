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
unsigned long intervalo_loop=100;
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
    Serial.begin(9600);
    myservo_X.attach(9);
    reloj.begin();
    estado=1;
    estado_termo=0;
    start=1;
    //ajustaReloj(2021, 7, 8, 4, 59, 50);
    //reloj.adjust(DateTime(__DATE__, __TIME__));
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
        printFecha();
        printVars();
        fecha = reloj.now();
        switch(estado) {
            case 1:
                if ((unsigned long)(actualMillis - previoMillisAUTO) >= 2000) {
                    Serial.println(estado);
                    if((((fecha.hour() >= horaON1)   && (fecha.hour() < horaOFF1)) ||
                        ((fecha.hour() >= horaON2) && (fecha.hour() < horaOFF2))) && (estado_termo==0)) {
                        termoACC(posON1, posON2);
                        Serial.println("TERMO ON");
                    } else if((((fecha.hour() < horaON1)   || (fecha.hour() >= horaOFF1)) && 
                               ((fecha.hour() < horaON2) || (fecha.hour() >= horaOFF2))) && (estado_termo==1)) {
                        termoACC(posOFF1, posOFF2);
                        Serial.println("TERMO OFF");
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
                if(validaPOS(posTEMP)) {
                    myservo_X.write(posTEMP);
                    estado_termo=0;
                }
                // debe reaccionar a los parametros enviados por Serial
                // movimiento libre, termoACC, ajustaReloj, configs, etc.
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
                Serial.print("Datos leidos :"+(String)di+"/"+(String)me+"/"+(String)an+" "+(String)hr+":"+(String)mn+":"+(String)sg);
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
    } else {
        Serial.println("Nada q hacer");
    }
}



void limpiaSerial() {
    while(Serial.available() > 0) {
        char temp = Serial.read();
    }
}

//como bash: cut -d
//String getValue(String data, char separator, int index){
//    int found = 0;
//    int strIndex[] = { 0, -1 };
//    int maxIndex = data.length() - 1;
//    for (int i = 0; i <= maxIndex && found <= index; i++) {
//        if (data.charAt(i) == separator || i == maxIndex) {
//            found++;
//            strIndex[0] = strIndex[1] + 1;
//            strIndex[1] = (i == maxIndex) ? i+1 : i;
//        }
//    }
//    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
//}

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
        default:
            Serial.print("default setVarsEP");
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
        Serial.println("Set RTC: valido");
    } else {
        Serial.println("Set RTC: invalido");
    }
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
  Serial.print(getHora('s'));
  Serial.print(" Día de la semana: ");
  Serial.println((String)fecha.dayOfTheWeek());
}

void printVars() {
    Serial.println("Variables: "+(String)horaON1+", "+(String)horaOFF1+", "+
                                 (String)horaON2+", "+(String)horaOFF2+", "+
                                 (String)posON1+", "+(String)posON2+", "+
                                 (String)posOFF1+", "+(String)posOFF2);
}
