import serial
import pandas as pd
import time

PORT = 'COM22'
df   = []

arduino = serial.Serial(PORT, 9600, timeout=5)
print('conectado!')
time.sleep(2)
arduino.write('1'.encode())


def getValue():
    global arduino

    try:
        return eval(arduino.readline().decode('utf-8'))
    except:
        return None

while True:
    data = getValue()
    print(data)

    if data is None:
        continue
    
    df.append(data)

    if data['time'] > 100:
        break

df = pd.DataFrame(df)
df.to_csv('DataBase.csv', index=False)
