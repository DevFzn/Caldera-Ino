#!/bin/bash

IP_CALDERA="<IP_ESP01>"

estado_caldera() {
    RESP=$(curl -s ${IP_CALDERA})
    arRS=(${RESP//,/ })
    echo "=============================="
    echo "         Estado Termo         "
    [[ ${arRS[1]} = "1" ]] && echo "         - ENCENDIDO -        "
    [[ ${arRS[1]} = "0" ]] && echo "         -  APAGADO  -        "
    echo "------------------------------"
    case ${arRS[0]} in
        '1')
            echo "        Modo Automático       "
            ;;
        '5')
            echo "          Modo Manual         "
            ;;
        '6')
            echo "          Modo Libre          "
            ;;
        *)
            echo "      verificar conexión!     "
            ;;
    esac
    for i in {10..14}; do [[ ${arRS[${i}]} -lt 10 ]] && arRS[${i}]="0"${arRS[${i}]}; done
    echo "     Hora  :   ${arRS[10]}:${arRS[11]}:${arRS[12]}"
    echo "     Fecha :   ${arRS[13]}/${arRS[14]}/${arRS[15]}"
    echo -e "-------------------------------"
    for i in {2..5}; do [[ ${arRS[${i}]} -lt 10 ]] && arRS[${i}]=" "${arRS[${i}]}; done
    echo " 1ra Hora de encend.  : ${arRS[2]} hrs"
    echo " 1ra Hora de apagado  : ${arRS[3]} hrs"
    echo " 2ra Hora de encend.  : ${arRS[4]} hrs"
    echo " 2ra Hora de apagado  : ${arRS[5]} hrs"
    echo -e "-------------------------------"
    for i in {6..9}; do [[ ${arRS[${i}]} -lt 100 ]] && arRS[${i}]=" "${arRS[${i}]}; done
    echo " 1ra Posición encend. :  ${arRS[6]}"
    echo " 2da Posición encend. :  ${arRS[7]}"
    echo " 1ra Posición apagado :  ${arRS[8]}"
    echo " 2da Posición apagado :  ${arRS[9]}"
    echo -e "===============================\n"
}

ejec_orden() {
    ORD=""
    case $1 in
        2)
            ORD="/sethora?1=2&2=${2}&3=${3}&4=${4}&5=${5}&6=${6}&7=${7}"
            ;;
        3)
            ORD="/horasAcc?1=3&2=${2}&3=${3}&4=${4}&5=${5}"
            ;;
        4)
            ORD="/setservo?1=4&2=${2}&3=${3}&4=${4}&5=${5}"
            ;;
        5)
            if [ ${2} = "on" ]
            then
                ORD="/accion?1=5&2=on"
            elif [ ${2} = "off" ]
            then
                ORD="/accion?1=5&2=off"
            fi
            ;;
        6)
            ORD="/setlibre?1=6&2=${1}"
            ;;
        *)
            ORD=""
            ;;
      esac
      curl -s "${IP_CALDERA}${ORD}"
}

principal() {
    clear
    estado_caldera
    echo "  1) Funcionamiento Autonomo"
    echo "  2) Configurar hora y fecha"
    echo "  3) Conf. hrs. de funcionamiento"
    echo "  4) Conf. posiciones de func."
    echo "  5) Modo manual"
    echo "  6) Modo libre"
    echo "  0) Salir"
    echo 
    read -p "  Ingresa Opcion :" OPC
    case ${OPC} in
        "1")
            curl -s "${IP_CALDERA}/auto"
            ;;
        "2")
            echo -e "______________________________\n"
            echo " Configuracion de fecha y hora"
            read -p "ingresa el DIA: " DIA
            read -p "ingresa el MES: " MES
            read -p "ingresa el AÑO: " ANO
            read -p "ingresa la HORA: " HRA
            read -p "ingresa los MINUTOS: " MIN
            read -p "ingresa los SEGUNDOS: " SEG
            ejec_orden 2 ${DIA} ${MES} ${ANO} ${HRA} ${MIN} ${SEG}
            ;;
        "3")
            echo -e "______________________________\n"
            echo " Conf. horas de funcionamiento"
            read -p " 1ra hora de encendido: " HR_ON1
            read -p " 1ra hora de apagado  : " HR_OFF1
            echo " opcional"
            read -p " 2da hora de encendido: " HR_ON2
            read -p " 2da hora de apagado  : " HR_OFF2
            ejec_orden 3 ${HR_ON1} ${HR_OFF1} ${HR_ON2} ${HR_OFF2}
            ;;
        "4")
            echo -e "______________________________\n"
            echo " Conf. posiciones del servo"
            read -p " 1ra posición encendido: " POS_ON1
            read -p " 2ra posición encendido: " POS_ON2
            read -p " 1ra posición apagado: " POS_OFF1
            read -p " 2ra posición apagado: " POS_OFF2
            ejec_orden 4 ${POS_ON1} ${POS_ON2} ${POS_OFF1} ${POS_OFF2}
            ;;
        "5")
            echo -e "______________________________\n"
            echo " Funcionamiento Manual "
            read -p " Encender o apagar (on/off) :" MANUAL
            ejec_orden 5 ${MANUAL}
            ;;
        "6")
            echo -e "______________________________\n"
            echo " Movimiento libre "
            read -p " ingresa posición (15<165):" MANUAL
            ejec_orden 6 ${MANUAL}
            ;;
        "0")
            exit
            ;;
    esac
sleep 8
principal
}

principal
exit
