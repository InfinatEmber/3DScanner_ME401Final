import serial
import time

def get_connection():
    baud = int(input("What is the baudrate? "))
    com = input("What is the comport? ")
    return serial.Serial(port=com,baudrate = baud, timeout = 0.1)
def write(arduino,x):
    arduino.write(bytes(x,'utf-8'))
def read(arduino):
    return arduino.readline()
def confirm_connection(arduino):
    write(arduino,"hi")
    while read(arduino) != "hi":
        write(arduino,"hi")
        time.sleep(0.05)



def run():
    print("Making a connection.")
    arduino = get_connection()
    print("Confirming the connection.")
    confirm_connection(arduino)
    