import serial
import time
import util


class Cameo25:
    """Class for representing the moving head"""

    def __init__(self, baudrate, debug=False):
        self.baudrate = baudrate
        self.__connect()
        self.debug = debug

    def __connect(self):
        port = util.pick_serial_port()
        self.serialConnection = serial.Serial(port, self.baudrate, timeout=10)
        self.serialConnection.flushInput()
        time.sleep(1)

    def __send(self, data):
        self.serialConnection.write((data + "\n").encode("utf-8"))
        if self.debug:
            print("Sent " + str(data))
        self.serialConnection.flush()

    def move_x(self, velocity=10):
        self.__send('x_vel ' + str(velocity))

    def stop_all(self):
        self.__send('x_vel 0')
        self.__send('y_vel 0')

    """Call this when you're done playing with it"""
    def die(self):
        self.serialConnection.close()
