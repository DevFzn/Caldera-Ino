#!/usr/bin/env python3.10

"""
    Script de control Caldera automatizada [Arduino]+[ESP01]
"""
import os
import sys
from time import sleep
import configparser as cfg
import vista_term as vt

try:
    parser = cfg.ConfigParser()
    parser.read('esp_conf.cfg')
    ESP01 = parser.get('esp01', 'URL')
except cfg.Error as ex:
    print('Error al leer archivo de configuración\n', ex)
    sys.exit()

try:
    import requests
except ModuleNotFoundError as ex:
    print("Debes instalar los modulos necesarios\n", ex)
    sys.exit()

clear = lambda: os.system('clear') if os.name == 'posix' else os.system('cls')

def enviar_consulta(consulta):
    """Retorna la respuesta a la petición GET *consulta*

    :consulta: str ( consulta creada por consultas() )
    :returns: str ( respuesta del GET request )
    """
    resp = requests.get(consulta)
    return resp.text

def consultas(modo, id_modo='', datos=['']):
    """Crea la petición GET según si los datos son list o str
    Retorna la respuesta de enviar_consulta()

    :modo: str (sub-menu)
    :id_modo: str (id modo de operacion)
    :datos: list[str] | str
    :returns: str (respuesta del GET request)
    """
    if id_modo != '':
        modo = modo + '?1=' + id_modo
    orden = ''
    cont = 2
    if type(datos) is list:
        if len(datos) == 1:
            orden = ''
        else:
            for dato in datos:
                if dato != '':
                    orden += '&' + str(cont) + '=' + str(dato)
                    cont += 1
    else:
        orden += '&' + str(cont) + '=' + str(datos)
    resp = enviar_consulta(ESP01 + modo + orden)
    return resp


intentos=0

def datos_a_lista() -> list[str] : 
    """Retorna el str recibido de consultas() como una lista, valida el largo
    de la lista, añade 0 o ' ' a valores, según sea necesario para el correcto
    formateo del texto a mostrar.

    :returns: list[str] ( len(list) = 16 )
    """
    datos = []
    global intentos
    while True:
        try:
            resp = consultas('')
            for var in resp.split(','):
                datos.append(var)
            assert len(datos) == 16
            for i in range(10,15):
                if int(datos[i]) < 10:
                    datos[i] = '0'+datos[i]
            for i in range(2,6):
                if int(datos[i]) < 10:
                    datos[i] = ' '+datos[i]
            intentos = 0
            return datos
        except KeyboardInterrupt:
            sys.exit()
        except AssertionError:
            vt.resp_conex('assert', intentos)
            intentos += 1
            sleep(2)
        except ConnectionError:
            vt.resp_conex('conexion', intentos)
            intentos += 1
            sleep(2)
        except OSError:
            vt.resp_conex('oserror', intentos)
            intentos += 1
            sleep(2)
        if intentos > 4:
            vt.resp_conex()
            sys.exit()


def main():
    global intentos
    while True:
        # Lista con valores de los datos recibidos
        vals = datos_a_lista() 
        clear()
        vt.pant_principal(vals[0], vals[1], vals[10:16], vals[2:6])
        modo_user = vt.entrada_usuario()
        match modo_user:
            case 's':
                sys.exit()
            case '1':
                #vt.pant_test_valores(vals)
                vt.respuesta_config(consultas('/auto'))
                intentos = 0
                sleep(3)
            case'2':
                vt.pant_config_fecha(vals[10:16])
                resp_user = vt.entrada_usuario('/sethora')
                vt.respuesta_config(consultas('/sethora', modo_user, resp_user))
                intentos = 0
                sleep(5)
            case '3':
                vt.pant_termporizador(vals[2:6])
                resp_user = vt.entrada_usuario('/horasAcc')
                vt.respuesta_config(consultas('/horasAcc', modo_user, resp_user))
                intentos = 0
                sleep(5)
            case '4':
                pos_servo = []
                for val in vals[6:10]:
                    if int(val) < 100:
                        pos_servo.append(' '+val)
                    else:
                        pos_servo.append(val)
                vt.pant_posic_servo(pos_servo)
                resp_user = vt.entrada_usuario('/setservo')
                vt.respuesta_config(consultas('/setservo', modo_user, resp_user))
                intentos = 0
                sleep(5)
            case '5':
                vt.pant_accion_manual(vals[1])
                resp_user = vt.entrada_usuario('/accion')
                vt.respuesta_config(consultas('/accion', modo_user, resp_user))
                intentos = 0
                sleep(5)
            case '6':
                vt.pant_servo_manual()
                resp_user = vt.entrada_usuario('/setlibre')
                vt.respuesta_config(consultas('/setlibre', modo_user, resp_user))
                intentos = 0
                sleep(5)
            case '':
                vt.respuesta_info()
                sleep(1.5)
            case 'v':
                vt.respuesta_info('volver')
                intentos = 0
                sleep(1.5)
            case _:
                modo_user = vt.entrada_usuario()
                vt.respuesta_info('error')
                sleep(1.5)


if __name__ == "__main__":
    main()
