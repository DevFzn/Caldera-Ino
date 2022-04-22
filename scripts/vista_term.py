"""
    Gestión de pantallas (dialogos, opcs, menus, etc) y entradas de usuario.
"""
import os
import sys
from time import sleep
from caldera import main
try:
    from colorama import Fore, Back, Style
except ModuleNotFoundError as ex:
    print("Debes instalar los modulos necesarios\n")
    print(ex)
    sleep(2)
    sys.exit()

clear = lambda: os.system('clear') if os.name == 'posix' else os.system('cls')

# Colores
col_rst      = Style.RESET_ALL
col_menu     = Fore.BLACK+Back.LIGHTBLACK_EX+Style.DIM
col_head     = Fore.BLUE+Back.LIGHTWHITE_EX+Style.NORMAL
col_headGrn  = Fore.GREEN+Back.MAGENTA+Style.BRIGHT
col_headRed  = Fore.RED+Back.BLUE+Style.DIM
col_headLred = Fore.LIGHTRED_EX+Back.BLUE+Style.DIM
col_headBBlk = Fore.BLACK+Back.MAGENTA+Style.BRIGHT
col_headBlk  = Fore.BLACK+Back.MAGENTA+Style.NORMAL
col_date     = Fore.LIGHTGREEN_EX+Back.LIGHTBLACK_EX
col_error    = Fore.RED+Back.YELLOW+Style.NORMAL
col_errorB   = Fore.RED+Back.YELLOW+Style.BRIGHT
col_input    = Fore.BLACK+Back.LIGHTBLACK_EX+Style.DIM
col_excep    = Fore.BLUE+Back.LIGHTBLACK_EX+Style.DIM
col_resp     = Fore.BLACK+Back.MAGENTA+Style.BRIGHT

# Dialogos
diag_bar    = '==========================='
diag_spc    = '                           '
diag_state  = [ diag_bar, '  ESTADO TERMO  ', ' ENCENDIDO ', '  APAGADO  ', '  - - - -  ' ]
diag_modo   = [ '  MODO          ', '  AUTO     ', '  MANUAL   ',
                '  LIBRE    ', '  ¡ERROR!  ' ]
diag_hrsfun = [ '  ON-1  ', ' OFF-1 ', '  ON-2  ', ' OFF-2 ', diag_bar ]
diag_opcs   = [ diag_bar, '     Menu de Opciones      ', diag_spc,
                '  1) Modo Autonomo         ', '  2) Ajustar hora y fecha  ',
                '  3) Ajustar temporizador  ', '  4) Calibrar servo        ',
                '  5) Modo Manual           ', '  6) Modo Libre            ',
                '  v) Volver(en sub-menus)  ', '  s) Salir                 ',
                '  ⏎) Actualizar            ', diag_spc, 
                ' Ingresa una opción        \b\b\b\b\b\b:' ]

diag_setdate = [ diag_bar,'  Valores actuales en RTC  ', '   Hora  :   ', '   Fecha :   ',
                 diag_bar, diag_spc, '  Ajustando fecha y hora   ', diag_spc,
                 '        DIA       :        \b\b\b\b\b\b\b',
                 '        MES       :        \b\b\b\b\b\b\b',
                 '        AÑO       :        \b\b\b\b\b\b\b',
                 '        HORA      :        \b\b\b\b\b\b\b',
                 '        MINUTO    :        \b\b\b\b\b\b\b',
                 '        SEGUNDO   :        \b\b\b\b\b\b\b'  ]

diag_error   = [ ' Ingresa un número válido! ', '  Debes ingresar \'on\' u \'off\' ',
                 '    Entrada no válida!     ', '    Entrada incorrecta!    ' ]

diag_settimer = [ diag_bar,'   Horario Temporizador    ', '  ON-1  ', ' OFF-1 ',
                  '  ON-2  ',' OFF-2 ', diag_bar, diag_spc, '   Ingresa nuevo horario   ',
                  diag_spc, '    1er Encendido  :       \b\b\b\b\b\b',
                  '    1er Apagado    :       \b\b\b\b\b\b',
                  '    2do Ecendido (opc.):   \b\b','    2do Apagado  (opc.):   \b\b' ]

diag_posservo = [ diag_bar,'   Configuracion Actual    ', '  1 Posición ON   ', 
                  '  2 Posición ON   ', '  1 Posición OFF  ','  2 Posición OFF  ',
                  diag_bar, diag_spc,'  Ingresa nuevos valores   ', diag_spc,
                  ' 1ra posic. encendido :    \b\b\b\b',
                  ' 2da posic. encendido :    \b\b\b\b',
                  ' 1ra posic. apagado   :    \b\b\b\b',
                  ' 2da posic. apagado   :    \b\b\b\b' ]

diag_accman  = [ diag_bar, '   Accionamiento Manual    ', diag_state[1:5],
                 diag_bar, diag_spc, '    Encender o Apagar ?    ', diag_spc,
                 ' Ingresa \'on\' u \'off\':     \b\b\b\b' ]

diag_servman = [ diag_bar, '     Movimiento Libre      ', diag_bar, diag_spc, 
                 'Posiciónes validas (16-164)', diag_spc ,
                 '   Ingresa posición :      \b\b\b\b\b' ]

diag_excepcn = [ diag_spc, ' Error en lectura de datos ', '  Arduino no disponible    ',
                 '  Error al solicitar datos ', f'  Intentando conexión (', ')  ',
                 ' Imposible conectar con    ', ' ESP o Arduino, verifica   ',
                 ' IP, conexión o estado de  ', ' de los dispositivos.      ' ]

diag_volver  = [ diag_spc, '      Actualizando...      ',
                '        Volviendo...       ', '    Seleción incorrecta    ' ]

# Valores para validación
#                 (min, max) dia - mes - año - hora - minuto - segundo
valid_dates     = [(0,32),(0,13),(2020,2100),(-1,24),(-1,60),(-1,60)]
valid_opcs      = ['1','2','3','4','5','6','']
valid_hrs_timer = (-1,24)
valid_posicion  = (15,168)
valid_manual    = ('on', 'off')


def resp_conex(excepcion=None, intentos=0):
    """Imprime en terminal dialogos de error de conexión 

    :excepcion: str (tipo de error)
    :intentos: int (nro. de intentos de conexión)
    """
    intentos += 1
    dialogos = diag_excepcn
    match excepcion:
        case 'assert':
            clear()
            print(col_menu + dialogos[0] + col_rst)
            print(col_error + dialogos[1] + col_rst)
            print(col_menu + dialogos[4] + str(intentos) + dialogos[5] + col_rst)
            print(col_menu + dialogos[0] + col_rst)
        case 'conexion':
            clear()
            print(col_menu + dialogos[0] + col_rst)
            print(col_error + dialogos[2] + col_rst)
            print(col_menu + dialogos[4] + str(intentos) + dialogos[5] + col_rst)
            print(col_menu + dialogos[0] + col_rst)
        case 'oserror':
            clear()
            print(col_menu + dialogos[0] + col_rst)
            print(col_error + dialogos[3] + col_rst)
            print(col_menu + dialogos[4] + str(intentos) + dialogos[5] + col_rst)
            print(col_menu + dialogos[0] + col_rst)
        case _:
            clear()
            print(col_menu + dialogos[0] + col_rst)
            for dialogo in dialogos[6:10]:
                print(col_errorB + dialogo + col_rst)
            print(col_menu + dialogos[0] + col_rst)


def pant_servo_manual():
    """Imprime en terminal dialogos de configuración manual del servo
    """
    clear()
    dialogos = diag_servman[:-1]
    for dialogo in dialogos: 
        print(col_menu+dialogo+col_rst)


def pant_accion_manual(estado):
    """Imprime en terminal dialogos de accionamiento manual del servo

    :estado: str ('0' o '1')
    """
    clear()
    dialogos = diag_accman[0:7]
    for ind, dialogo in enumerate(dialogos):
        match ind:
            case 2:
                print(col_head+dialogo[0]+col_rst, end='')
                match estado:
                    case '1':
                        print(col_headGrn+dialogo[1]+col_rst)
                    case '0':
                        print(col_headBlk+dialogo[2]+col_rst)
                    case _:
                        print(col_headBlk+dialogo[3]+col_rst)
                ind+=3
            case _:
                print(col_menu+dialogo+col_rst)


def pant_posic_servo(pos_servo):
    """Imprime en terminal posiciones de trabajo del servo

    :pos_servo: list[str]
    """
    clear()
    dialogos = diag_posservo[0:10]
    for ind, dialogo in enumerate(dialogos):
        match ind:
            case 2:
                print(f"{col_head}{dialogo}{col_headGrn}  {pos_servo[0]}    {col_rst}")
            case 3:
                print(f"{col_head}{dialogo}{col_headGrn}  {pos_servo[1]}    {col_rst}")
            case 4:
                print(f"{col_head}{dialogo}{col_headGrn}  {pos_servo[2]}    {col_rst}")
            case 5:
                print(f"{col_head}{dialogo}{col_headGrn}  {pos_servo[3]}    {col_rst}")
            case _:
                print(col_menu + dialogo + col_rst) 


def pant_termporizador(hrs_timer):
    """Imprime en terminal horas de trabajo del servo

    :hrs_timer: list[str]
    """
    clear()
    dialogos = diag_settimer[0:10]
    for ind, dialogo in enumerate(dialogos):
        match ind:
            case 2:
                #ind+=1
                print(f"{col_head}{dialogo}{col_headGrn}  {hrs_timer[0]}  {col_rst}", end='')
            case 3:
                print(f"{col_head}{dialogo}{col_headGrn}  {hrs_timer[1]}  {col_rst}")
            case 4:
                print(f"{col_head}{dialogo}{col_headGrn}  {hrs_timer[2]}  {col_rst}", end='')
            case 5:
                print(f"{col_head}{dialogo}{col_headGrn}  {hrs_timer[3]}  {col_rst}")
            case _:
                print(col_menu + dialogo + col_rst) 


def pant_config_fecha(tiempo):
    """Imprime hora y fecha en terminal, segun ESP.

    :tiempo: list[str] [hora, minuto, segundo, dia, mes, año]
    """
    clear()
    dialogos = diag_setdate
    for ind, dialogo in enumerate(dialogos):
        match ind:
            case 2:
                print(f"{col_date}{dialogo}{tiempo[0]}:{tiempo[1]}:{tiempo[2]}      {col_rst}")
            case 3:
                print(f"{col_date}{dialogo}{tiempo[3]}/{tiempo[4]}/{tiempo[5]}    {col_rst}")
            case 8:
                break
            case _:
                print(col_menu + dialogo + col_rst) 


def pant_principal(modo, estado, tiempo, hrs_timer):
    """Imprime menu principal en terminal.

    :modo: str (ej. '\\n')
    :estado: str estado del termo ('0','1')
    :tiempo: List[str] hora y fecha
    :hrs_timer: List[str] horario de accionamiento
    """
    clear()
    # Estado
    dialogos = diag_state
    print(col_menu+dialogos[0]+col_rst)
    print(col_head+dialogos[1]+col_rst, end='')
    match estado:
        case '1':
            print(col_headGrn+dialogos[2]+col_rst)
        case '0':
            print(col_headBlk+dialogos[3]+col_rst)
        case _:
            print(col_headBlk+dialogos[4]+col_rst)

    # Modo de operación
    dialogos = diag_modo
    print(col_head+dialogos[0]+col_rst, end='')
    match modo:
        case '\n1':
            print(col_headGrn+dialogos[1]+col_rst)
        case '\n5':
            print(col_headBlk+dialogos[2]+col_rst)
        case '\n6':
            print(col_headBlk+dialogos[3]+col_rst)
        case _:
            print(col_headRed+dialogos[4]+col_rst)
    
    # Hora y Fecha
    print(col_date, end='')
    print(f"   {tiempo[0]}:{tiempo[1]}:{tiempo[2]}", end='')
    print(f"   {tiempo[3]}/{tiempo[4]}/{tiempo[5]}   "+col_rst)

    # Horas de funcionamiento
    dialogos = diag_hrsfun
    print(col_head + dialogos[0] + col_rst, end='')
    print(col_headBBlk + '  ' + hrs_timer[0] + '  ' + col_rst, end='')
    print(col_head + dialogos[1]+col_rst, end='')
    print(col_headBBlk + '  ' + hrs_timer[1] + '  ' + col_rst)
    print(col_head + dialogos[2] + col_rst, end='')
    print(col_headBBlk + '  ' + hrs_timer[2] + '  ' + col_rst, end='')
    print(col_head + dialogos[3]+col_rst, end='')
    print(col_headBBlk + '  ' + hrs_timer[3] + '  ' + col_rst)

    # Opciones del menu principal
    dialogos = diag_opcs[:-1]
    for indx, dialogo in enumerate(dialogos):
        if indx != (len(dialogos)-1):
            print(col_menu+dialogo+col_rst)
        else:
            print(col_menu+dialogo+col_rst)


def pant_test_valores(valores):
    """Función de prueba, imprime en terminal valores de lista.

    :valores: Lista de strings [ str, str, ...]
    """
    for val in valores:
        print(Fore.MAGENTA+Back.BLACK+Style.BRIGHT+val+Style.RESET_ALL)


def valida_entrada(valid, mensaje, tipo_dato='') -> list[str] | str:
    """Solicita entrada de usuario, imprime dialogo en terminal y 
    valida según tupla *valid*.

    :valid: tuple (límites o alternativas válidas)
    :mensaje: str (mensaje para solicitar entrada de usuario)
    """
    invalido = True
    err_msg = ''
    while invalido:
        entrada_usuario = input(col_input+mensaje+Style.RESET_ALL)
        if entrada_usuario == 's':
            sys.exit()
            #return 's'
        elif entrada_usuario == 'v':
            main()
            #return 'v'
        match tipo_dato:
            case '':
                try:
                    entrada_usuario = int(entrada_usuario)
                    if valid[0] < entrada_usuario < valid[1]:
                        invalido = False
                        return str(entrada_usuario)
                    else:
                        raise ValueError
                except ValueError:
                    err_msg = diag_error[0]
                    print(col_error + err_msg + Style.RESET_ALL)
            case 'str':
                try:
                    if entrada_usuario in valid:
                        invalido = False
                        return entrada_usuario
                    else:
                        err_msg = diag_error[3]
                        raise ValueError
                except ValueError:
                    err_msg = diag_error[3]
                    print(col_error + err_msg + Style.RESET_ALL)
            case _:
                assert entrada_usuario in ['','str']
                err_msg = diag_error[2]
                print(col_error + err_msg + Style.RESET_ALL)


def entrada_usuario(caso=None) -> list[str] | str :
    """Retorna la(s) respuesta(s) del usuario, segun caso (submenu)

    :caso: str (submenu)
    """
    match caso:
        case '/sethora':
            user_ins = []
            i = 0
            for preg in diag_setdate[8:14]:
                user_ins.append(valida_entrada(valid_dates[i], preg))
                i += 1
            return user_ins
        case '/horasAcc':
            user_ins = []
            for preg in diag_settimer[10:14]:
                user_ins.append(valida_entrada(valid_hrs_timer, preg))
            return user_ins
        case '/setservo':
            user_ins = []
            for preg in diag_posservo[10:14]:
                user_ins += valida_entrada(valid_posicion, preg)
            return user_ins
        case '/accion':
            return valida_entrada(valid_manual ,diag_accman[-1],'str')
        case '/setlibre':
            return valida_entrada(valid_posicion ,diag_servman[-1])
        case _:
            mensaje = ' Ingresa una opción        \b\b\b\b\b\b:'
            return valida_entrada(valid_opcs, mensaje, 'str')


def respuesta_config(respuesta):
    """Imprime el argumento formateado en el terminal bajo el contexto de 
    respuesta del ESP a alguna configuración.
    
    :respuesta: str (mensaje de respuesta)
    """
    print(col_resp + respuesta + col_rst)

def respuesta_info(caso='foo'):
    """Imprime en terminal bajo el contexto de respuesta informativa o 
    advertencia del ESP a alguna solicitud.
    
    :caso: str (tipo de mensaje)
    """
    match caso:
        case 'error':
            print(col_error + diag_volver[3] + col_rst)
            sleep(1)
        case 'volver':
            print(col_error + diag_volver[2] + col_rst)
            sleep(1)
        case _:
            print(col_error + diag_volver[1] + col_rst)
            sleep(1)
 
