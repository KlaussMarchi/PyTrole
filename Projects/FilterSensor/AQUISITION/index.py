import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import serial # pip install pyserial
from time import sleep, time
import pandas as pd
import keyboard

def conectarArduino(tempoEspera=1, porta=1):
    conexao = 'COM' + str(porta)
    
    try:
        arduino = serial.Serial(conexao, 9600, timeout=tempoEspera)
    except:
        print('ERRO NA CONEXÃO NA PORTA ' + conexao)
        sleep(tempoEspera)
        return conectarArduino(tempoEspera, porta+1)

    if arduino.isOpen():
        arduino.close()
        
    arduino.open()
    return arduino


def getValue():
    global arduino

    try:
        return float(arduino.readline().decode('utf-8'))
    except:
        return 0
    

porta = int(input('digite a porta: '))
arduino = conectarArduino(porta=porta)

print('ARDUINO CONECTADO')
input('APERTE ENTER PARA INICIAR A LEITURA E ENTER PARA PARAR')

To   = time()
data = []

while True:
    value = getValue()
    t = time() - To
    
    print(value)
    data.append([value, t])

    if keyboard.is_pressed('enter') or len(data) > 1000000:
        break

df = pd.DataFrame(data, columns=['time', 'analog'])
df.to_csv('file.csv')