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
