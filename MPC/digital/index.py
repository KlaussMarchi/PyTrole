import numpy as np
import time
import scipy.optimize
import scipy.signal
import control as ctl
import matplotlib.pyplot as plt
import serial

# Parâmetros do controlador e do sistema
M = 10  # Horizonte de controle
P = 20  # Horizonte de predição
dt = 1  # Taxa de amostragem

Xo = 0
Ts = 1
setpoint = 6
bounds = [(-500, 500) for _ in range(M)]

s = ctl.TransferFunction.s
G = 1/(s**2 + 2*s)
num, den = (G.num[0][0], G.den[0][0])
G = scipy.signal.lti(num, den)

matrix = G.to_ss()
n  = matrix.A.shape[0]
x0 = Xo * np.ones(n)

tData = np.array([])
rData = np.array([])

device = None
port = 'COM13'



def extendArray(array, newSize):
    n = newSize - len(array)
    newData = np.repeat(array[-1], n)
    return np.concatenate([array, newData])


def forcedResponse(G, U, T, Xo):
    return scipy.signal.lsim(G, U=U, T=T, X0=Xo, interp=False)[1]


def getError(u, T, Xo):
    yData = forcedResponse(G, U=extendArray(u, P), T=T, Xo=Xo)
    return sum((rData - yData)**2)


def getOutputs():
    global rData, tData
    Xo = x0[0]
    tData = np.arange(0, P*dt, dt)
    rData = Xo + (setpoint - Xo) * (1 - np.exp(-4*Ts * tData))
    uData = np.ones(M)
    result = scipy.optimize.minimize(getError, uData, args=(tData, x0), bounds=bounds)
    return np.zeros(P) if abs(setpoint - Xo) < 0.1 else result.x 


def getInputs():
    response = getDeviceMsg()

    if not response:
        return None

    if response.find('input') == -1:
        return None

    response = response[response.find(':')+1:].strip()

    if len(response) == 0:
        return None

    return float(response) * np.ones(n)


def setOutputs(outputs):
    msg = 'outputs' + str([np.round(output, 3) for output in outputs])
    print(msg)
    return setDeviceMsg(msg)


def getDeviceMsg(numeric=False):
    global device
    value = None
    
    if device.in_waiting == 0:
        return value

    try:
        response = device.readline().decode('utf-8')
        start = response.find('$')
        end   = response.find('!')

        if start == -1 or end == -1:
            return False

        response = response[start+1:end].strip()

        if len(response) == 0:
            return False

        print('received:', response)
        value = float(response) if numeric else response
    except Exception as e:
        print(e)

    return value


def setDeviceMsg(msg):
    global device
    response = False

    try:
        device.write(('$' + msg.strip() + '!').encode())
        response = True
    except Exception as e:
        print(e)

    return response


def setup():
    global device
    device = serial.Serial(port, 9600, timeout=10)
    time.sleep(5)


def loop():
    global x0
    x0 = getInputs()

    if x0 is None:
        return

    print(f'input: {x0[0]} | setpoint: {setpoint}')
    
    outputs = getOutputs()
    setOutputs(outputs)



if __name__ == '__main__':
    setup()

    while True:
        loop()

