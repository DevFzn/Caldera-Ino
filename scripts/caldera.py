#!/usr/bin/python3
"""
    Script de control Caldera autmatizada Arduino+ESP01
"""
import os
import sys
from time import sleep
try:
    import requests
    from colorama import Fore, Back, Style
except ModuleNotFoundError as ex:
    print("Debes instalar los modulos necesarios\n")
    print(ex)
    sleep(2)
    sys.exit()

ESP01 = "http://192.168.31.247"
clear = lambda: os.system('clear') if os.name == 'posix' else os.system('cls')

def enviar_consulta(consulta):
    resp = requests.get(consulta)
    return resp.text

def consultas(modo, *args):
    orden = '?'
    for indx, arg in enumerate(args):
        orden += str(indx+1)+'='+str(arg)+'&'
    resp = enviar_consulta(ESP01+modo+orden[:-1])
    return resp

def pantallas(pantalla, vals):
    if pantalla == '':
        if vals[0] == '\n1':
            vals[0] = '  AUTO     '
        elif vals[0] == '\n5':
            vals[0] = '  MANUAL   '
        elif vals[0] == '\n6':
            vals[0] = '  LIBRE    '
        else:
            vals[0] = '¡¡¡ERROR!!!'
        for i in range(10,15):
            if int(vals[i]) < 10:
                vals[i] = '0'+vals[i]
        for i in range(2,6):
            if int(vals[i]) < 10:
                vals[i] = ' '+vals[i]
        clear()
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        if vals[1] == '1':
            print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  ESTADO TERMO  "+Style.RESET_ALL+
                  Fore.GREEN+Back.BLUE+Style.BRIGHT+f" ENCENDIDO " + Style.RESET_ALL)
        elif vals[1] == '0':
            print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  ESTADO TERMO  "+Style.RESET_ALL+
                  Fore.RED+Back.BLUE+Style.BRIGHT+f"  APAGADO  " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  MODO          "+Style.RESET_ALL+
              Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"{vals[0]}" + Style.RESET_ALL)
        print(Fore.LIGHTGREEN_EX+Back.LIGHTBLACK_EX+
              f"   {vals[10]}:{vals[11]}:{vals[12]}   {vals[13]}/{vals[14]}/{vals[15]}   "+
              Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  ON-1  "+Style.RESET_ALL+
              Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[2]}  " + Style.RESET_ALL, end='')
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+" OFF-1 "+Style.RESET_ALL+
              Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[3]}  " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  ON-2  "+Style.RESET_ALL+
              Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[4]}  " + Style.RESET_ALL, end='')
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+" OFF-2 "+Style.RESET_ALL+
              Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[5]}  " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"     Menu de Opciones      "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" 1.- Modo Autonomo         "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" 2.- Ajustar hora y fecha  "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" 3.- Ajustar temporizador  "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" 4.- Calibrar servo        "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" 5.- Modo Manual           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" 6.- Modo Libre            "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" 0.- Salir                 "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" ⏎.- Actualizar            "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" Ingresa una opción        "+Style.RESET_ALL,
              end='\b\b\b\b\b\b')
    elif pantalla == '1':
        print(Fore.MAGENTA+Back.BLUE+Style.BRIGHT+vals+Style.RESET_ALL)
    elif pantalla == '2':
        for i in range(0,5):
            if int(vals[i]) < 10 and vals[i][0:1] != '0' :
                vals[i] = '0'+vals[i]
        clear()
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"  Valores actuales en RTC  "+Style.RESET_ALL)
        print(Fore.LIGHTGREEN_EX+Back.LIGHTBLACK_EX+
              f"   Hora  :   {vals[0]}:{vals[1]}:{vals[2]}      "+Style.RESET_ALL)
        print(Fore.LIGHTGREEN_EX+Back.LIGHTBLACK_EX+
              f"   Fecha :   {vals[3]}/{vals[4]}/{vals[5]}    "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"  Ajustando fecha y hora   "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
    elif pantalla == '3':
        for i in range(0,4):
            if int(vals[i]) < 10 and vals[i][0:1] != ' ':
                vals[i] = ' '+vals[i]
        clear()
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"   Horario Temporizador    "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  ON-1  "+Style.RESET_ALL+
          Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[0]}  " + Style.RESET_ALL, end='')
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+" OFF-1 "+Style.RESET_ALL+
          Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[1]}  " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  ON-2  "+Style.RESET_ALL+
          Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[2]}  " + Style.RESET_ALL, end='')
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+" OFF-2 "+Style.RESET_ALL+
          Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[3]}  " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"   Ingresa nuevo horario   "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
    elif pantalla == '4':
        for i in range(0,4):
            if int(vals[i]) < 100:
                vals[i] = ' '+vals[i]
        clear()
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"   Configuracion Actual    "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  1 Posición ON   "+Style.RESET_ALL+
          Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[0]}    " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  2 Posición ON   "+Style.RESET_ALL+
          Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[1]}    " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  1 Posición OFF  "+Style.RESET_ALL+
          Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[2]}    " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  2 Posición OFF  "+Style.RESET_ALL+
          Fore.LIGHTRED_EX+Back.BLUE+Style.BRIGHT+f"  {vals[3]}    " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"  Ingresa nuevos valores   "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
    elif pantalla == '5':
        clear()
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"   Accionamiento Manual    "+Style.RESET_ALL)
        if vals[0] == '1':
            print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  ESTADO TERMO  "+Style.RESET_ALL+
                  Fore.GREEN+Back.BLUE+Style.BRIGHT+f" ENCENDIDO " + Style.RESET_ALL)
        elif vals[0] == '0':
            print(Fore.BLUE+Back.LIGHTWHITE_EX+Style.DIM+"  ESTADO TERMO  "+Style.RESET_ALL+
                  Fore.RED+Back.BLUE+Style.BRIGHT+f"  APAGADO  " + Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"     Encender o Apagar     "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
    elif pantalla == '6':
        clear()
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"    Movimiento Libre       "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"==========================="+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+" Ingresa posición (16..164)"+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)

def solicita_dato(valid, mensaje):
    invalido = True
    while invalido:
        try:
            dato = input(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+mensaje+Style.RESET_ALL)
            dato = int(dato)
            if valid[0] < dato < valid[1]:
                invalido = False
                return dato
            raise ValueError
        except ValueError:
            print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+
                  "Ingresa un número válido  "+Style.RESET_ALL)

def solicita_dato_str(valid, mensaje):
    invalido = True
    while invalido:
        try:
            dato = input(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+mensaje+Style.RESET_ALL)
            if dato in valid:
                invalido = False
                return dato
            raise ValueError
        except ValueError:
            print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+
                  "  Debes ingresar \'on\' u \'off\' "+Style.RESET_ALL)

intentos = 0
while True:
    #res[1] = 'ENCENDIDO' if res[1] == '1' else 'APAGADO' if '0' else ''
    res = []
    try:
        clear()
        resp = consultas('')
        for var in resp.split(','):
            res.append(var)
        assert len(res) == 16
        pantallas('', res)
        opc = input(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+":"+Style.RESET_ALL)
        if opc == '0':
            sys.exit()
        elif opc == '1':
            resp = consultas('/auto')
            print(Fore.MAGENTA+Back.BLUE+Style.BRIGHT+resp+Style.RESET_ALL)
            sleep(8)
        elif opc == '2':
            pantallas('2',[res[10], res[11], res[12], res[13], res[14], res[15]])
            resp = consultas('/sethora', opc, solicita_dato((0,32),'        DIA       : '),
                                              solicita_dato((0,13),'        MES       : '),
                                              solicita_dato((2020,2100),'        AÑO       : '),
                                              solicita_dato((-1,24),'       HORA       : '),
                                              solicita_dato((-1,60),'       MINUTO     : '),
                                              solicita_dato((-1,60),'       SEGUNDO    : '))
            print(Fore.MAGENTA+Back.BLUE+Style.BRIGHT+resp+Style.RESET_ALL)
            sleep(8)
        elif opc == '3':
            pantallas('3',[res[2], res[3], res[4], res[5]])
            resp = consultas('/horasAcc', opc,
                                         solicita_dato((-1,24),'    1er Encendido      : '),
                                         solicita_dato((-1,24),'    1er Apagado        : '),
                                         solicita_dato((-1,24),'    2do Ecendido (opc.): '),
                                         solicita_dato((-1,24),'    2do Apagado  (opc.): '))
            print(Fore.MAGENTA+Back.BLUE+Style.BRIGHT+resp+Style.RESET_ALL)
            sleep(8)
        elif opc == '4':
            pantallas('4',[res[6], res[7], res[8], res[9]])
            resp = consultas('/setservo', opc,
                                          solicita_dato((15,165),' 1ra Posición Encendido:'),
                                          solicita_dato((15,165),' 2ra Posición Encendido:'),
                                          solicita_dato((15,165),' 1ra Posición Apagado  :'),
                                          solicita_dato((15,165),' 2da Posición Apagado  :'))
            print(Fore.MAGENTA+Back.BLUE+Style.BRIGHT+resp+Style.RESET_ALL)
            sleep(8)
        elif opc == '5':
            pantallas('5',[res[1]])
            resp = consultas('/accion', opc, solicita_dato_str(('on','off'),
                             ' Ingresa \'on\' u \'off\': '))
            print(Fore.MAGENTA+Back.BLUE+Style.BRIGHT+resp+Style.RESET_ALL)
            sleep(8)
        elif opc == '6':
            pantallas('6',[res[1]])
            resp = consultas('/setlibre', opc, solicita_dato((15,165),'  Ingresa posición     :'))
            print(Fore.MAGENTA+Back.BLUE+Style.BRIGHT+resp+Style.RESET_ALL)
            sleep(8)
        elif opc == '':
            print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+
                  Style.RESET_ALL)
            print(Fore.RED+Back.YELLOW+Style.BRIGHT+"      Actualizando...      "+
                  Style.RESET_ALL)
            print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+
                  Style.RESET_ALL)
            sleep(1.5)
        else:
            print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+
                  Style.RESET_ALL)
            print(Fore.RED+Back.YELLOW+Style.BRIGHT+"     Opción incorrecta     "+
                  Style.RESET_ALL)
            print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+
                  Style.RESET_ALL)
            sleep(1.5)
    except KeyboardInterrupt:
        sys.exit()
    except AssertionError:
        clear()
        intentos += 1
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.RED+Back.YELLOW+Style.BRIGHT+"  Arduino no disponible    "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+f"  Intentando nuevamente ({intentos+1}) "+
              Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        intentos += 1
        sleep(2)
    except ConnectionError:
        clear()
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+f"  Intentando conexión ({intentos+1})  "+
              Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        intentos += 1
        sleep(2)
    except OSError:
        clear()
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+f"  Intentando conexión ({intentos+1})  "+
              Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        intentos += 1
        sleep(2)
    if intentos > 4:
        print(Fore.RED+Back.YELLOW+Style.BRIGHT+" Imposible conectar con    "+Style.RESET_ALL)
        print(Fore.RED+Back.YELLOW+Style.BRIGHT+" ESP, o Arduino, verfica   "+Style.RESET_ALL)
        print(Fore.RED+Back.YELLOW+Style.BRIGHT+" IP, conexión o estado de  "+Style.RESET_ALL)
        print(Fore.RED+Back.YELLOW+Style.BRIGHT+     " de los dispositivos.      "+Style.RESET_ALL)
        print(Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM+"                           "+Style.RESET_ALL)
        sys.exit()


#if __name__ == '__main__':
