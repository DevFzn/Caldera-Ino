#!/bin/bash

IP_CALDERA="<IP_ESP01>"

estado_caldera() {
    RESP=$(curl -s ${IP_CALDERA})
    MODO=$(echo ${RESP} | cut -d',' -f 1)
    ESTADO=$(echo ${RESP} | cut -d',' -f 2)
    HR_ON1=$(echo ${RESP} | cut -d',' -f 3)
    HR_OFF1=$(echo ${RESP} | cut -d',' -f 4)
    HR_ON2=$(echo ${RESP} | cut -d',' -f 5)
    HR_OFF2=$(echo ${RESP} | cut -d',' -f 6)
    POS_ON1=$(echo ${RESP} | cut -d',' -f 7)
    POS_ON2=$(echo ${RESP} | cut -d',' -f 8)
    POS_OFF1=$(echo ${RESP} | cut -d',' -f 9)
    POS_OFF2=$(echo ${RESP} | cut -d',' -f 10)
    HRA=$(echo ${RESP} | cut -d',' -f 11)
    MIN=$(echo ${RESP} | cut -d',' -f 12)
    SEG=$(echo ${RESP} | cut -d',' -f 13)
    DIA=$(echo ${RESP} | cut -d',' -f 14)
    MES=$(echo ${RESP} | cut -d',' -f 15)
    ANO=$(echo ${RESP} | cut -d',' -f 16)
    echo "============================="
    echo "    Estado Termo"
    [[ ${ESTADO} = "1" ]] && echo "   - ENCENDIDO -"
    [[ ${ESTADO} = "0" ]] && echo "   -  APAGADO  -"
    echo -e "-----------------------------"
    case $MODO in
        '1')
            echo "  Modo Automático"
            ;;
        '5')
            echo "  Modo Manual (esperando orden)"
            ;;
        '6')
            echo "  Modo Libre"
            ;;
        *)
            echo "  verificar conexión!"
            ;;
    esac
    echo "  Hora : ${HRA}:${MIN}:${SEG}"
    echo "  Fecha: ${DIA}/${MES}/${ANO}"
    echo -e "-------------------------------"
    echo " 1ra Hora de encend.  : ${HR_ON1} hrs"
    echo " 1ra Hora de apagado  : ${HR_OFF1} hrs"
    echo " 2ra Hora de encend.  : ${HR_ON2} hrs"
    echo " 2ra Hora de apagado  : ${HR_OFF2} hrs"
    echo -e "-------------------------------"
    echo " 1ra Posición encendido: ${POS_ON1}"
    echo " 2da Posición encendido: ${POS_ON2}"
    echo " 1ra Posición apagado  : ${POS_OFF1}"
    echo " 2da Posición apagado  : ${POS_OFF2}"
    #echo -e "-----------------------------\n"
    echo -e "=============================\n"
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
            echo "______________________________"
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
            echo "______________________________"
            echo " Conf. horas de funcionamiento"
            read -p " 1ra hora de encendido: " HR_ON1
            read -p " 1ra hora de apagado  : " HR_OFF1
            echo " opcional"
            read -p " 2da hora de encendido: " HR_ON2
            read -p " 2da hora de apagado  : " HR_OFF2
            ejec_orden 3 ${HR_ON1} ${HR_OFF1} ${HR_ON2} ${HR_OFF2}
            ;;
        "4")
            echo "______________________________"
            echo " Conf. posiciones del servo"
            read -p " 1ra posición encendido: " POS_ON1
            read -p " 2ra posición encendido: " POS_ON2
            read -p " 1ra posición apagado: " POS_OFF1
            read -p " 2ra posición apagado: " POS_OFF2
            ejec_orden 4 ${POS_ON1} ${POS_ON2} ${POS_OFF1} ${POS_OFF2}
            ;;
        "5")
            echo "_______________________" 
            echo " Funcionamiento Manual "
            read -p " Encender o apagar (on/off) :" MANUAL
            ejec_orden 5 ${MANUAL}
            ;;
        "6")
            echo "__________________" 
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
