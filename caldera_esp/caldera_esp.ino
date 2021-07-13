#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "confidencial.h"
String modo, estado, horaOn1, horaOff1, horaOn2, horaOff2, posOn1, posOn2, posOff1, posOff2, hora, minuto, segundo, dia, mes, ano;

ESP8266WebServer server(80);
const int led = 13;

void handleRoot() {
    String mensaje, msj_arduino, valores_ino;
    valores_ino = "Modo: "+modo+"\nEstado: "+estado+"\nHora encendido: "+horaOn1+"\nHora apagado: "+horaOff1+
                "\nHora encendido2: "+horaOn2+"\nHora apagado2: "+horaOff2+"\nPosiciones Servo ON1 y ON2: "+posOn1+", "+posOn2+
                "\nPosiciones Servo OFF1 y OFF2: "+posOff1+", "+posOff2+"\nHora y fecha: "+hora+":"+minuto+":"+segundo+"  "+dia+
                "/"+mes+"/"+ano;
    for (uint8_t i = 0; i < server.args(); i++) {
        mensaje += " " + server.argName(i) + ": " + server.arg(i) + "\n";
        msj_arduino+=server.arg(i)+",";
    }
    server.send(200, "text/plain", "Argumentos: "+mensaje+"\nValores Arduino :\n"+valores_ino);
    Serial.println(msj_arduino);
    limpiaSerial();
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
    pinMode(led, OUTPUT);
    digitalWrite(led, 0);
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

void loop(void) {
    if (Serial.available() > 0) {
        modo = Serial.readStringUntil(',');
        estado = Serial.readStringUntil(',');
        if(estado == "1") { 
            estado = "Encendido"; 
        } else if(estado == "0") { 
            estado = "Apagado"; 
        }
        horaOn1 = Serial.readStringUntil(',');
        horaOff1 = Serial.readStringUntil(',');
        horaOn2 = Serial.readStringUntil(',');
        horaOff2 = Serial.readStringUntil(',');
        posOn1 = Serial.readStringUntil(',');
        posOn2 = Serial.readStringUntil(',');
        posOff1 = Serial.readStringUntil(',');
        posOff2 = Serial.readStringUntil(',');
        hora = Serial.readStringUntil(',');
        minuto = Serial.readStringUntil(',');
        segundo = Serial.readStringUntil(',');
        dia = Serial.readStringUntil(',');
        mes = Serial.readStringUntil(',');
        ano = Serial.readStringUntil(',');
    }
    limpiaSerial();
    server.handleClient();
}

