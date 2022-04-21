# Utilidades  
  Scripts para enviar peticiones web al ESP.

## Bash
```
─ 📂️ scripts
  ├── 📄️ caldera.sh
  └── 📄️ esp_conf.cfg
```

Archivo de configuración
***./esp_conf.cfg***
```cfg
[esp01]
URL = <direcion_ESP>
```
Dependencias
- curl
- grep
- cut

Correr: `  ./caldera.sh  `

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
----

## Python
versión ***Python >= 3.10***  
```
─ 📂️ scripts
  ├── 📄️ caldera.py
  ├── 📄️ esp_conf.cfg
  └── 📄️ vista_term.py
```

Archivo de configuración
***./esp_conf.cfg***
```cfg
[esp01]
URL = <direcion_ESP>
```

Dependencias:
- requests
- colorama

Correr: ` ./caldera.py ` o ` python caldera.py `

![python_script](./script_python.png)
