#include <Servo.h>
#include <RTClib.h>

Servo myservo_X;
RTC_DS3231 reloj;

volatile DateTime fecha;
unsigned long intervalo_loop=5000;
unsigned long previoMillisTermo=0;
unsigned long previoMillisLoop=0;
unsigned long actualMillis;
byte horaON = 5;
byte horaOFF = 35;
byte cont=0;
bool estado_termo;
bool espera;
bool start;

void setup() {
  myservo_X.attach(9);
  Serial.begin(115200);               // COMENTAR ******
  reloj.begin();
  start=1;
  estado_termo = 0;
}
//reloj.adjust(DateTime(__DATE__, __TIME__));

void loop() {
  if(start) {
    myservo_X.write(30);
    Serial.print("Servo a CERO");
    start=0;
  }
  actualMillis = millis();
  if ((unsigned long)(actualMillis - previoMillisLoop) >= intervalo_loop) {
    Serial.println("LOOP");         // COMENTAR ******
    Serial.println("Consulta RTC"); // COMENTAR ******
    fecha = reloj.now();            // COMENTAR ******
    mostrarFechaSerie();            // COMENTAR ******
    Serial.print("Estado Termo en LOOP = ");
    Serial.println(estado_termo);
    if((fecha.second() >= horaON) && (fecha.second() < horaOFF) && (estado_termo == 0)) {
      termoON();
      Serial.print("Termo = ");
      Serial.println(estado_termo);
    } else if(((fecha.second() < horaON) || (fecha.second() >= horaOFF)) && (estado_termo == 1)) {
      termoOFF();
      Serial.print("Termo = ");
      Serial.println(estado_termo);
    }
    previoMillisLoop = millis();
  }
}

// Secuencia de encendido y apagado de Caldera
void termoON() {
  Serial.println("termoON() :");
  myservo_X.write(120);
  Serial.println(" S120 ");
  espera = 0;
  cont = 0;
  previoMillisTermo = millis();
  Serial.print("do while -> contador y posicion servo : ");
  do{
    actualMillis = millis();
    if(espera == 0) {
      if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
        myservo_X.write(110);
        previoMillisTermo = millis();
        espera = 1;
        cont++;
        Serial.print("   Servo=110 - cont=");
        Serial.print(cont);
      }
    }
    if(espera == 1) {
      if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
        myservo_X.write(120);
        previoMillisTermo = millis();
        espera = 0;
        cont++;
        Serial.print("   Servo=120 - cont=");
        Serial.print(cont);
      }      
    }
  } while (cont < 4);
  Serial.println("");
  estado_termo = 1;
}

void termoOFF() {
  Serial.println("termoOFF() :");
  myservo_X.write(45);
  Serial.println(" S45 ");
  espera = 0;
  cont = 0;
  previoMillisTermo = millis();
  Serial.print("do while -> contador y posicion servo : ");
  do{
    actualMillis = millis();
    if(espera == 0) {
      if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
        myservo_X.write(62);
        previoMillisTermo = millis();
        espera = 1;
        cont++;
        Serial.print("   Servo=62 - cont=");
        Serial.print(cont);
      }
    }
    if(espera == 1) {
      if ((unsigned long)(actualMillis - previoMillisTermo) >= 500) {
        myservo_X.write(45);
        previoMillisTermo = millis();
        espera = 0;
        cont++;
        Serial.print("   Servo=145 - cont=");
        Serial.print(cont);
      }      
    }
  } while (cont < 4);
  Serial.println("");
  estado_termo = 0;
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
void mostrarFechaSerie() {
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
}
