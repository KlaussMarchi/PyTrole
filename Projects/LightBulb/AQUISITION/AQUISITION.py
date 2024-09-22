import serial
import pandas as pd
import time

PORT = 'COM22'

arduino = serial.Serial(PORT, 9600, timeout=5)
allData = []
time.sleep(2)
arduino.write('1'.encode())


def getValue():
    global arduino

    try:
        return eval(arduino.readline().decode('utf-8'))
    except:
        print('erro')
        return None

while True:
    data = getValue()

    if data is None:
        continue
    
    print(data)
    allData.append(data)

    if data['time'] > 30:
        break

df = pd.DataFrame(allData)
df.to_csv('ANALISYS/DataBase.csv', index=False)
