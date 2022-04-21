## Control de encendido para caldera electrica
**Hardware**
- [ATmega328p](https://en.wikipedia.org/wiki/Arduino_Nano) (ArduinoNano old bootloader)  
- [Real time clock](https://en.wikipedia.org/wiki/Real-time_clock) (RTC-DS3231)  
- [Servo](https://es.wikipedia.org/wiki/Servomotor)  


Enciende o apaga el termo según configuración. Admite hasta dos horarios  
de funcionamiento al día, ajustables en **modo 3**.  

El funcionamiento autonomo es la opción por defecto (**modo 1**).  

Funcionamiento manual espera instrucción para realizar acción de encendido  
o apagado (**modo 5**).  

Hora y fecha ajustables en el **modo 2**.  

Posiciones del servo al encender o apagar el termo (**modo 4**). Son dos posiciones,  
ya que el servo realiza movimiento repetitivo para asegurar el accionamiento  
mecánico del interruptor del termo.  

El **modo 6** permite mover libremente el servo (desde 16 hasta 144). Útil para probar ajustes.  

## Arduino
Control de horario de encendido caldera electrica.  
Trabaja de forma autonoma y/o comandado por puerto serie (**115200 baudio**).    

* 3 Modos de Operación
  - Automatizado (**1**, por defecto)
  - Manual (**5**) 
  - Libre (**6**)
* 3 Modos de Configuración
  - Configuración fecha y hora (**2**)
  - Configuración horas de encendido y apagado (**3**)
  - Configuración de posiciones de encendido y apagado (**4**)


Arduino envia cada segundo los valores actuales de las variables de control al puerto serie.  
ej: `1,0,5,7,16,18,120,90,45,62,1,42,24,14,7,2021`
```
1____________________________________________ modo
| 0__________________________________________ estado termo (O=apagado, 1=Encendido)
| | 5________________________________________ hora de encendido
| | | 7______________________________________ hora de apagado
| | | | 16___________________________________ 2da hora de encendido (opcional)
| | | |  | 18________________________________ 2da hora de apagado (opcional)
| | | |  |  | 120____________________________ 1ra posicion encendido
| | | |  |  |  |  90_________________________ 2ra posicion encendido
| | | |  |  |  |  |  45______________________ 1ra posicion apagado
| | | |  |  |  |  |  |  62___________________ 2ra posicion apagado
| | | |  |  |  |  |  |  |  1_________________ hora en RTC
| | | |  |  |  |  |  |  |  | 42______________ minutos en RTC
| | | |  |  |  |  |  |  |  |  | 24___________ segundos en RTC
| | | |  |  |  |  |  |  |  |  |  | 14________ dia en RTC
| | | |  |  |  |  |  |  |  |  |  |  | 7______ mes en RTC
| | | |  |  |  |  |  |  |  |  |  |  | | 2021_ año en RTC
1,0,5,7,16,18,120,90,45,62,1,42,24,14,7,2021
```

### Configuración

Envío de datos a modificar por puerto serie de Arduino (**115200 baudio**).  
ejemplos: 

- Configurar hora **19:35:15** y fecha **19/04/2022**.
  - `2,19,4,2022,19,35,15`
- Configurar horas de funcionamiento de **6 a 8** y **17 a 18 hrs**.
  - `3,6,8,17,18`
- Configurar posiciones de funcionamiento, encendido **120 y 105**, apagado **45 y 62**.
  - `4,120,105,45,62`
- Cambiar a modo manual y apagar termo.
  - `5,0`
- Cambiar a modo libre y mover el servo a posición **100**.
  - `6,100`
- Cambiar a modo autónomo (**modo 1**).
  - `1`

