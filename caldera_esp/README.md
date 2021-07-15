ESP01
=====

Archivo de configuracion ***confidencial.h***
```c
const char* ssid = "SSID";
const char* password = "password";
```

## Urls
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

