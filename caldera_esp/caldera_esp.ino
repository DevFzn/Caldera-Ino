#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "confidencial.h"

String modo, estado, horaOn1, horaOff1, horaOn2, horaOff2, posOn1, posOn2, posOff1, posOff2, hora, minuto, segundo, dia, mes, ano;
String temporal;

ESP8266WebServer server(80);

void handleRoot() {
    server.send(200, "text/plain", temporal);
}

void handleACCmanual() {
    String mensaje;
    if(server.arg(0) == "5") {
        if(server.arg(1) == "on") {
            mensaje = "Encendiendo Caldera";
            Serial.println("5,1");
        } else if(server.arg(1) == "off") {
            mensaje = "Apagando Caldera";
            Serial.println("5,0");
        } else {
            mensaje = "Opcion no valida\n<mod=5&acc= >";
        }
    } else {
        mensaje = "Opcion no valida ?,?";
    }
    server.send(200, "text/plain", mensaje);
}

void handleSEThora() {
    // dia mes año hora min seg
    String mensaje_web, msj_a_arduino;
    if(server.arg(0) == "2") {
        msj_a_arduino = "2,";
        msj_a_arduino += server.arg(1)+",";
        msj_a_arduino += server.arg(2)+",";
        msj_a_arduino += server.arg(3)+",";
        msj_a_arduino += server.arg(4)+",";
        msj_a_arduino += server.arg(5)+",";
        msj_a_arduino += server.arg(6);
        mensaje_web = "Fecha y hora enviadas a arduino";
        Serial.println(msj_a_arduino);
    } else { 
        mensaje_web = "Opcion no valida\n<mod=2&dia= &mes= &año= &hrs= &mins= &segs= >";
    }
    server.send(200, "text/plain", mensaje_web);
}

void handleHorasAcc() {
    String mensaje_web, msj_a_arduino;
    if(server.arg(0) == "3") {
        msj_a_arduino = "3,";
        msj_a_arduino += server.arg(1)+",";
        msj_a_arduino += server.arg(2)+",";
        msj_a_arduino += server.arg(3)+",";
        msj_a_arduino += server.arg(4);
        mensaje_web = "Horas de funcionamiento enviadas a arduino";
        Serial.println(msj_a_arduino);
    } else { 
        mensaje_web = "Opcion no valida\n<mod=3&hrOn1= &hrOff1= &hrOn2= &hrOff2= >";
    }
    server.send(200, "text/plain", mensaje_web);
}

void handleSETservo() {
    String mensaje_web, msj_a_arduino;
    if(server.arg(0) == "4") {
        msj_a_arduino = "4,";
        msj_a_arduino += server.arg(1)+",";
        msj_a_arduino += server.arg(2)+",";
        msj_a_arduino += server.arg(3)+",";
        msj_a_arduino += server.arg(4);
        mensaje_web = "Posiciones del servo enviadas a arduino";
        Serial.println(msj_a_arduino);
    } else { 
        mensaje_web = "Opcion no valida\n<mod=4&posOn1= &posOn2= &posOff1= &posOff2= >";
    }
    server.send(200, "text/plain", mensaje_web);
}

void handleSETlibre() {
    String mensaje_web, msj_a_arduino;
    if(server.arg(0) == "6") {
        msj_a_arduino = "6,";
        msj_a_arduino += server.arg(1);
        mensaje_web = "Posicion del servo enviada a arduino";
        Serial.println(msj_a_arduino);
    } else {
        mensaje_web = "Opcion no valida\n<mod=6&posServ= >";
    }
    server.send(200, "text/plain", mensaje_web);
}

void handleSETauto() {
    String mensaje_web, msj_a_arduino;
    msj_a_arduino = "1";
    mensaje_web = "Enviando modo 1 (Autonomo) a arduino";
    Serial.println(msj_a_arduino);
    server.send(200, "text/plain", mensaje_web);
}

void handleNotFound() {
    String message = "Archivo no encontrado\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setup(void) {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    
    server.on("/accion", handleACCmanual);
    server.on("/sethora", handleSEThora);
    server.on("/horasAcc", handleHorasAcc);
    server.on("/setservo", handleSETservo);
    server.on("/setlibre", handleSETlibre);
    server.on("/auto", handleSETauto);

    server.on("/inline", []() {
        server.send(200, "text/plain", "esto funciona como debe");
    });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void limpiaSerial() {
    while(Serial.available() > 0) {
        char temp = Serial.read();                       
    }
}

void leerSerial() {
    while(Serial.available()){
        temporal = Serial.readStringUntil('\r');
    } 
}

void loop(void) {
    leerSerial();
    server.handleClient();
}

