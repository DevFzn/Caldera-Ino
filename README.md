# Automatizacion de Caldera

    Enciende o apaga el termo según configuración. Admite hasta dos   
    horarios de funcionamiento al día, ajustables en modo 3.  

    El funcionamiento autonomo es la opción por defecto (modo 1).  

    Funcionamiento manual espera instrucción para realizar acción de  
    encendido o apagado (modo 5).

    Hora y fecha ajustables en el modo 2

    Posiciones del servo al encender o apagar el termo (modo 4).  
    Son dos posiciones, ya que el servo realiza movimiento repetitivo  
    con para asegurar el accionamiento mecánico del interruptor del termo.

    El modo 6 permite mover libremente el servo (desde 16 hasta 144).  
    Útil para probar ajustes.  






Hardware utilizado:
- ATmega328p (ArduinoNano old bootloader)  
- RTC-DS3231  
- Servo  
- esp8266 (ESP01)  

Arduino funciona de forma autonoma, no requiere del módulo wifi.  
El ESP-01 funciona como interface web entre el usuario y el puerto serie de arduino.  

caldera.sh: utilidad para enviar controlar caldera por terminal.  

-----

## Control de horario de encendido caldera electrica
Trabaja de forma autonoma y/o comandado por puerto serie (**115200 baudio**).    

* 3 Modos de Operación
  - Automatizado (1, por defecto)
  - Manual (5) 
  - Libre (6)
* 3 Modos de Configuracion
  - Configuracion de fecha y hora (2)
  - Configuracion horas de encendido y apagado (3)
  - Configuracion de posiciones de encendido y apagado (4)


Arduino envia cada segundo los valores actuales de las variables de control al puerto serie.
```
    ej.
    1,0,5,7,16,18,120,90,45,62,1,42,24,14,7,2021

    1 > modo, 
    | 0 > estado termo (O = apagado, 1 = Encendido)
    | | 5 > hora de encendido
    | | | 7 > hora de apagado
    | | | | 16 > 2da hora de encendido (opcional)
    | | | |  | 18 > 2da hora de apgado (opcional)
    | | | |  |  | 120 > 1ra posicion encendido
    | | | |  |  |  |  90 > 2ra posicion encendido
    | | | |  |  |  |  |  45 > 1ra posicion apagado
    | | | |  |  |  |  |  |  62 > 2ra posicion apagado
    | | | |  |  |  |  |  |  |  1 > hora en RTC
    | | | |  |  |  |  |  |  |  | 42 > minutos en RTC
    | | | |  |  |  |  |  |  |  |  | 24 > segundos en RTC
    | | | |  |  |  |  |  |  |  |  |  | 14 > dia en RTC
    | | | |  |  |  |  |  |  |  |  |  |  | 7 > mes en RTC
    | | | |  |  |  |  |  |  |  |  |  |  | | 2021 > año en RTC
    1,0,5,7,16,18,120,90,45,62,1,42,24,14,7,2021
```
-----

## ESP01

Establece comunicación serial con arduino (**115200 baudio**).  
Interface web sencilla para consulta y configuraciones  

Archivo de configuracion ***confidencial.h***  
```c
const char* ssid = "SSID";
const char* password = "password";
```

#### Urls
* '**/**' : muestra valores actuales de variables.
    - ej. http://192.168.65.10/
* '**/accion**' : accionamiento manual, espera argumentos on u off.
    - ej. http://192.168.65.10/accion?mod=5&acc=on
* '**/sethora**' : configurar de fecha y hora.
    - ej. http://192.168.65.10/sethora?mod=2&dia=14&mes=7&año=2021&hrs=1&mins=33&segs=24 
* '**/horasAcc**' : configurar horas de funcionamiento.
    - ej. http://192.168.65.10/horasAcc?mod=3&hrOn1=5&hrOff1=7&hrOn2=16&hrOff2=18
* '**/setservo**' : configurar posiciones de encendido y apagado.
    - ej. http://192.168.65.10/setservo?mod=4&posOn1=110&posOn2=85&posOff1=45&posOff2=62
* '**/setlibre**' : Mover servo libremente (15 < posicion > 165).
    - ej. http://192.168.65.10/setlibre?mod=6&pos=125
* '**/auto**' : Modo autonomo.
    - ej. http://192.168.65.10/auto
-----

### caldera.sh
  Script para enviar peticiones web al ESP.

```
==============================
         Estado Termo         
         - ENCENDIDO -        
------------------------------
        Modo Automático       
------------------------------
     Hora  :   05:12:47
     Fecha :   16/07/2021
------------------------------
    Horario Temporizador   
    1)  ON:  5    OFF:  7 
    2)  ON: 16    OFF: 18 
==============================

  1) Funcionamiento Autonomo
  2) Ajustar hora y fecha
  3) Ajustar termporizador
  4) Calibrar Servo
  5) Modo Manual
  6) Modo Libre
  0) Salir

  Ingresa Opcion :

```
