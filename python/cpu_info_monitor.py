# Importing the library
from pickle import TRUE
from xml.dom.expatbuilder import parseString
import psutil
import time as tiempo
import os
import serial
import serial.tools.list_ports as port_list


puertos_disponibles = list(port_list.comports())
publicar_en_consola = True

for p in puertos_disponibles:
    print (p)

puerto_a_usar = "COM6"                                              #modificar con el puerto COM del arduino

ser = serial.Serial(puerto_a_usar)  # open serial port

os.system('cls')

#for i in range(60):
while(1):
    cant_cpu = int(psutil.cpu_percent(0.5))
    cant_ram = int(psutil.virtual_memory()[2])

    if publicar_en_consola:
        os.system('cls')
        print("\n")
        for i in range(100):
            if i < cant_cpu:
                print("#",end="")
            else:
                print(".",end="")
        print(f" {cant_cpu} %\n")

        for i in range(100):
            if i < cant_ram:
                print("#",end="")
            else:
                print(".",end="")

        print(f" {cant_ram} %")


    '''CPU'''
    linea1 = "CPU: "+ (str(" ") if cant_cpu<10 else str("")) + str(cant_cpu) + " %"
    linea2 = ""
    if(cant_cpu):
        linea2 = "#"
    for j in range(int(cant_cpu/5)):
        linea2 += "#"

    '''RAM'''
    linea3 = "RAM: "+ (str(" ") if cant_ram<10 else str("")) + str(cant_ram) + " %"
    #linea3 = f"RAM: {cant_ram} %"
    linea4 = ""
    if(cant_ram):
        linea4 = "#"
    for j in range(int(cant_ram/5)):
        linea4 += "#"

    while(len(linea1) < 20):
            linea1 += " "
    while(len(linea2) < 20):
            linea2 += " "
    while(len(linea3) < 20):
            linea3 += " "
    while(len(linea4) < 20):
            linea4 += " "

    mensaje_arduino = linea1 + linea2 + linea3 + linea4
    mensaje_arduino += '\n'
    ser.write(bytes(mensaje_arduino, 'utf-8'))
    #tiempo.sleep(3)
ser.close()

