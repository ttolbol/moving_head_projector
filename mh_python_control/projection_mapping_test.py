import cv2
import numpy as np
from Cameo25 import Cameo25
import time
import math
from projection_mapping import Projector

cameo = Cameo25(115200, debug=True)

dia = 582  # px
fov = 16.75  # deg
center = (964, 696)

projector = Projector(dia=dia, fov=fov, x0=center[0], y0=center[1], pan=0, tilt=-90, roll=0)

px_per_deg = dia/fov

base_speed = 300 * px_per_deg / (8.88 * 3)

tx = -(150/px_per_deg)
t0 = time.time()
t1 = 150 / (base_speed * 1)

#img = cv2.imread('C:\\Users\\thoma\\Documents\\mask.png')

# generate mask
mask = np.zeros((1440, 2560, 3), dtype=np.uint8)
cv2.circle(mask, center, int(dia*1.1*0.5), (255, 255, 255), -1)

img = np.zeros((1440, 2560, 3), dtype=np.uint8)

cv2.namedWindow('window', cv2.WND_PROP_FULLSCREEN)
cv2.setWindowProperty('window', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)


def update_img(img, rx, ry):
    img = np.zeros_like(img)
    projector.set_rotation(rx, ry)
    x0, y0 = projector.get_camera_coords(0, -2050, 50)
    x1, y1 = projector.get_camera_coords(0, -2050, 0)
    if abs(x0 - center[0]) <= (dia*0.55) and abs(y0 - center[1]) <= (dia*0.55) and abs(x1 - center[0]) <= (dia*0.55) and abs(y1 - center[1]) <= (dia*0.55):
        cv2.line(img, (int(x0), int(y0)), (int(x1), int(y1)), (255, 0, 0))

    x0, y0 = projector.get_camera_coords(50, -2050, 0)
    x1, y1 = projector.get_camera_coords(0, -2050, 0)
    if abs(x0 - center[0]) <= (dia * 0.55) and abs(y0 - center[1]) <= (dia * 0.55) and abs(x1 - center[0]) <= (dia * 0.55) and abs(y1 - center[1]) <= (dia * 0.55):
        cv2.line(img, (int(x0), int(y0)), (int(x1), int(y1)), (0, 0, 255))

    x0, y0 = projector.get_camera_coords(0, -2050, 0)
    if abs(x0 - center[0]) <= (dia * 0.55) and abs(y0 - center[1]) <= (dia * 0.55):
        cv2.circle(img, (int(x0), int(y0)), 30, (255, 255, 255))
    # img = np.bitwise_and(img, mask)
    cv2.imshow('window', img)


cameo.cmd('led_off')
cameo.home()
cameo.goto_xy(-85, -130, True)
cameo.home()
cameo.goto_xy(0, -90, True)
cameo.cmd('led_on')

x_vel = 5
y_vel = 10
cameo.move_x(x_vel)
cameo.move_y(y_vel)

# free move
cameo.cmd('tracking_on')
cameo.cmd('motors_off')
while True:
    cameo.serialConnection.reset_input_buffer()
    while cameo.serialConnection.in_waiting == 0:
        pass

    line = cameo.readline()
    if line.find(', ') == -1:
        continue

    x, y = line.split(', ')
    x = float(x)
    y = float(y)

    update_img(img, -x, y)
    key = cv2.waitKey(33)
    if key == ord('q'):
        break


# actuated
while False:
    cameo.serialConnection.reset_input_buffer()
    cameo.cmd('get_pos')

    while cameo.serialConnection.in_waiting == 0:
        pass

    line = cameo.readline()

    if line.find(', ') == -1:
        continue

    x, y = line.split(', ')
    x = float(x)
    y = float(y)
    
    if x > 5 and x_vel > 0:
        x_vel = -5
        cameo.move_x(x_vel)
    
    if x < -5 and x_vel < 0:
        x_vel = 5
        cameo.move_x(x_vel)

    if y > -85 and y_vel > 0:
        y_vel = -10
        cameo.move_y(y_vel)

    if y < -95 and y_vel < 0:
        y_vel = 10
        cameo.move_y(y_vel)
    
    update_img(img, -x, y)
    key = cv2.waitKey(33)
    if key == ord('q'):
        break

cameo.cmd('tracking_off')
cameo.cmd('led_off')
cameo.cmd('speed 120')
cameo.goto_xy(-85, -130, True)
