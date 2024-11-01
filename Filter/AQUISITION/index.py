import serial  # pip install pyserial
from time import sleep
import keyboard
import pandas as pd
import serial.tools.list_ports


def getValue():
    global device

    try:
        return eval(device.readline().decode('utf-8'))
    except:
        return None


path   = 'ANALYSIS/DataBase.csv'
device = serial.Serial('COM5', 9600, timeout=10)
list   = []

print('conectado!')
sleep(2)

device.write('start\r\n'.encode())
print('aguarde...')


while True:
    values = getValue()
    
    if keyboard.is_pressed('enter'):
        break

    if values is None:
        continue
    
    list.append(values)
    print(values)

df = pd.DataFrame(list)
df.to_csv(path, index=False)