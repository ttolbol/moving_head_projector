import serial
import time
import util


class Cameo25:
    '''Class for representing the moving head'''

    def __init__(self, baudrate, debug=False):
        self.baudrate = baudrate
        self.__connect()
        self.debug = debug

    def __connect(self):
        port = util.pick_serial_port()
        self.serialConnection = serial.Serial(port, self.baudrate, timeout=10)
        self.serialConnection.reset_input_buffer()
        self.serialConnection.reset_output_buffer()
        time.sleep(1)

    def __send(self, data):
        self.serialConnection.write((data + '\n').encode('utf-8'))
        if self.debug:
            print('Sent ' + str(data))

    def cmd(self, cmd):
        self.__send(cmd)

    def home(self, wait=True):
        self.__send('home')
        if wait:
            self.wait_for_position()

    def move_x(self, velocity=10):
        self.__send('x_vel {0:.4f}'.format(velocity))

    def move_y(self, velocity=10):
        self.__send('y_vel {0:.4f}'.format(velocity))

    def stop_all(self):
        self.__send('x_vel 0')
        self.__send('y_vel 0')

    '''Call this when you're done playing with it'''
    def die(self):
        self.serialConnection.close()

    def goto_x(self, x_pos, wait=False):
        self.__send('x_pos {0:.4f}'.format(x_pos))
        if wait:
            self.wait_for_position(x=True, y=False)

    def goto_y(self, y_pos, wait=False):
        self.__send('y_pos {0:.4f}'.format(y_pos))
        if wait:
            self.wait_for_position(x=False, y=True)

    def goto_xy(self, x_pos, y_pos, wait=False):
        self.__send('x_pos {0:.4f}'.format(x_pos))
        self.__send('y_pos {0:.4f}'.format(y_pos))
        if wait:
            self.wait_for_position()

    def wait_for_position(self, x=True, y=True):
        x_reached = not x
        y_reached = not y

        while not x_reached or not y_reached:
            line = self.readline(verbose=True).strip()
            if line == 'x_target_reached':
                x_reached = True
            elif line == 'y_target_reached':
                y_reached = True
                
    def get_pos(self):
        self.serialConnection.reset_input_buffer()
        self.cmd('get_pos')
        line = self.readline().strip()
        while line.find(', ') == -1:
            line = self.readline().strip()

        print(line)
        x, y = line.split(', ')
        x = float(x)
        y = float(y)
        return x, y
        
    def readline(self, timeout=30, verbose=False):
        start_time = time.time()
        end_time = start_time
        line = ''
        while end_time-start_time < timeout:
            ch = self.serialConnection.read().decode('utf-8')
            if ch == '\n':
                if verbose:
                    print(line)
                return line
            line = line + ch
        return None

    def reset_die(self):
        # Home positions before termination
        self.goto_xy(-270, -135, wait=True)
        self.serialConnection.close()