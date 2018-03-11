import serial
import glob
import time


def pick_serial_port():
    ports = glob.glob('/dev/tty.*')
    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    time.sleep(1)
    return result[-1]