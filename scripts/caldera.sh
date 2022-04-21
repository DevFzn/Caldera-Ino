#!/usr/bin/env bash

IP_CALDERA=$(grep URL esp_conf.cfg | cut -d'=' -f2)

estado_caldera() {
    RESP=$(curl -s ${IP_CALDERA})
    arRS=(${RESP//,/ })
    echo "=============================="
    echo "         Estado Termo         "
    [[ ${arRS[1]} = "1" ]] && echo "         - ENCENDIDO -        "
    [[ ${arRS[1]} = "0" ]] && echo "         -  APAGADO  -        "
    echo "------------------------------"
    case ${arRS[0]} in
        '1')    echo "        Modo Automático       "   ;;
        '5')    echo "          Modo Manual         "   ;;
        '6')    echo "          Modo Libre          "   ;;
        *)      echo "      verificar conexión!     "   ;;
    esac
    echo "------------------------------"
    for i in {10..14}; do [[ ${arRS[${i}]} -lt 10 ]] && arRS[${i}]="0"${arRS[${i}]}; done
    echo "     Hora  :   ${arRS[10]}:${arRS[11]}:${arRS[12]}"
    echo "     Fecha :   ${arRS[13]}/${arRS[14]}/${arRS[15]}"
    echo "------------------------------"
    for i in {2..5}; do [[ ${arRS[${i}]} -lt 10 ]] && arRS[${i}]=" "${arRS[${i}]}; done
    echo "    Horario Temporizador   "
    echo "    1)  ON: ${arRS[2]}    OFF: ${arRS[3]} "
    echo "    2)  ON: ${arRS[4]}    OFF: ${arRS[5]} "
    echo -e "==============================\n"
}

ejec_orden() {
    ORD=""
    case $1 in
        2)  ORD="/sethora?1=2&2=${2}&3=${3}&4=${4}&5=${5}&6=${6}&7=${7}"    ;;
        3)  ORD="/horasAcc?1=3&2=${2}&3=${3}&4=${4}&5=${5}" ;;
        4)  ORD="/setservo?1=4&2=${2}&3=${3}&4=${4}&5=${5}" ;;
        5)  if [ -n ${2} ]; then
                if [ ${2} = "on" ]; then
                    ORD="/accion?1=5&2=on"
                elif [ ${2} = "off" ]; then
                    ORD="/accion?1=5&2=off"
                fi
            else 
                return
            fi  ;;
        6)  ORD="/setlibre?1=6&2=${1}"  ;;
        *)  ORD=""  ;;
      esac
      curl -s "${IP_CALDERA}${ORD}"
}

principal() {
    clear
    estado_caldera
    echo "  1) Funcionamiento Autonomo"
    echo "  2) Ajustar hora y fecha"
    echo "  3) Ajustar termporizador"
    echo "  4) Calibrar Servo"
    echo "  5) Modo Manual"
    echo "  6) Modo Libre"
    echo "  0) Salir"
    echo 
    read -p "  Ingresa Opcion :" OPC
    case ${OPC} in
        "1")    echo; curl -s "${IP_CALDERA}/auto"  ;;
        "2")
            clear
            echo "============================"
            echo "  Valores Actuales en RTC    "
            echo "    Hora  :   ${arRS[10]}:${arRS[11]}:${arRS[12]}"
            echo "    Fecha :   ${arRS[13]}/${arRS[14]}/${arRS[15]}"
            echo -e "============================\n"
            echo -e " Configuracion fecha y hora \n"
            read -p "ingresa el DIA: " DIA
            read -p "ingresa el MES: " MES
            read -p "ingresa el AÑO: " ANO
            read -p "ingresa la HORA: " HRA
            read -p "ingresa los MINUTOS: " MIN
            read -p "ingresa los SEGUNDOS: " SEG
            ejec_orden 2 ${DIA} ${MES} ${ANO} ${HRA} ${MIN} ${SEG}
            ;;
        "3")
            clear
            echo -e "=============================="
            echo "    Horario Temporizador   "
            echo "    1)  ON: ${arRS[2]}    OFF: ${arRS[3]} "
            echo "    2)  ON: ${arRS[4]}    OFF: ${arRS[5]} "
            echo -e "==============================\n"
            echo -e "  ingresa nuevo horario  \n"
            read -p " 1ra hora de encendido: " HR_ON1
            read -p " 1ra hora de apagado  : " HR_OFF1
            echo " opcional"
            read -p " 2da hora de encendido: " HR_ON2
            read -p " 2da hora de apagado  : " HR_OFF2
            ejec_orden 3 ${HR_ON1} ${HR_OFF1} ${HR_ON2} ${HR_OFF2}
            ;;
        "4")
            for i in {6..9}; do [[ ${arRS[${i}]} -lt 100 ]] && arRS[${i}]=" "${arRS[${i}]}; done
            clear
            echo -e "============================"
            echo "    Configuración actual     "
            echo "----------------------------"
            echo "  1ra Posicion ON  : ${arRS[6]}"
            echo "  2da Posición ON  : ${arRS[7]}"
            echo "----------------------------"
            echo "  1ra Posición OFF : ${arRS[8]}"
            echo "  2da Posición OFF : ${arRS[9]}"
            echo -e "============================\n"
            echo -e "     Ingresar  valores      \n"
            read -p " 1ra posición encendido: " POS_ON1
            read -p " 2ra posición encendido: " POS_ON2
            read -p " 1ra posición apagado: " POS_OFF1
            read -p " 2ra posición apagado: " POS_OFF2
            echo
            ejec_orden 4 ${POS_ON1} ${POS_ON2} ${POS_OFF1} ${POS_OFF2}
            ;;
        "5")
            clear
            echo -e "============================\n"
            echo -e "    Accionamiento Manual    "
            [[ ${arRS[1]} = "1" ]] && echo "        - ENCENDIDO -        "
            [[ ${arRS[1]} = "0" ]] && echo "        -  APAGADO  -        "
            echo -e "============================\n"
            echo -e " Encender o apagar (on/off)\n" 
            read -p " :" MANUAL
            echo
            ejec_orden 5 ${MANUAL}
            ;;
        "6")
            clear
            echo -e "============================\n"
            echo -e "      Movimiento libre      \n"
            echo -e "============================\n"
            echo " ingresa posición (15<165):"
            read -p " :" MANUAL
            echo
            ejec_orden 6 ${MANUAL}
            ;;
        "0")    exit    ;;
    esac
sleep 8
principal
}

principal
exit
