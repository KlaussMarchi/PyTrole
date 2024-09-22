import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import serial  # pip install pyserial
from time import sleep, time
import csv
import keyboard
import os

PATH = 'files/DataBaseFinal.csv'


# FUNÇÃO QUE IRÁ DEFINIR O EIXO VERTICAL DO GRÁFICO DE PONTOS
def getValue(arduino):
    try:
        return float(arduino.readline().decode('utf-8'))
    except:
        return 0


# CONECTANDO O COMPUTADOR COM UM ARDUINO DE PORTA NÃO ESPECIFICADA
def conectarArduino(tempoEspera=1, porta=1):
    conexao = 'COM' + str(porta)
    try:
        arduino = serial.Serial(conexao, 115200, timeout=tempoEspera)
    except:
        print('ERRO NA CONEXÃO NA PORTA ' + conexao)
        sleep(tempoEspera)
        return conectarArduino(tempoEspera, porta + 1)
    
    if arduino.isOpen():
        arduino.close()
        arduino.open()
    
    return arduino


def createTable():
    if not os.path.exists(PATH):
        with open(PATH, 'w', newline='') as csvfile:
            csvwriter = csv.writer(csvfile)
            csvwriter.writerow(['Temperatura', 'Umidade', 'Analogico', 'Hora', 'Sopro'])

def writeCSV(lista):
    with open(PATH, 'a', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(lista)


createTable()
PORTA = input('digite a porta: ')
arduino = conectarArduino(porta=int(PORTA))
To = time()

while True:
    value = getValue(arduino)
    
    if keyboard.is_pressed('enter'):
        break

    t = time() - To
    writeCSV([value, t])
    print([value, t])
    
 