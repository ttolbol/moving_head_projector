import cv2
import numpy as np
from Cameo25 import Cameo25
import time
from projection_mapping import Projector
from projection_mapping import Texture

cameo = Cameo25(115200, debug=True)

dia = 582  # px
fov = 16.75  # deg
center = (964, 696)

dist = 2600

projector = Projector(dia=dia, fov=fov, x0=center[0], y0=center[1], pan=0, tilt=-90, roll=0)

textures = []
textures.append(Texture('images\\xray_1.jpg', -430, -2580, 160, 0.32))
textures.append(Texture('images\\hand.png', -730, -2580, 120, 120/920))
textures.append(Texture('images\\xray_2.jpg', -1110, -2580, 100, 0.6))
textures.append(Texture('images\\splat.png', -1480, -2580, 0, 0.7))
textures.append(Texture('images\\xray_3.jpg', -1850, -2580, 50, 0.43))

# generate mask
mask = np.zeros((1440, 2560, 3), dtype=np.uint8)
cv2.circle(mask, center, int(dia*1.1*0.5), (255, 255, 255), -1)

img = np.zeros((1440, 2560, 3), dtype=np.uint8)

cv2.namedWindow('window', cv2.WND_PROP_FULLSCREEN)
cv2.setWindowProperty('window', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
cv2.imshow('window', mask)


def update_img(img, rx, ry):
    img = np.zeros_like(img)
    projector.set_rotation(-rx, ry)
    for t in textures:
        img = t.render(projector, img)
    # cv2.circle(img, center, int(dia * 1.1 * 0.5), (255, 255, 255), 1)
    # img = np.bitwise_and(img, mask)
    cv2.imshow('window', img)


cameo.cmd('led_off')
cameo.home()
cameo.goto_xy(-85, -130, True)
cameo.home()

x = -12
y = -90
cameo.goto_xy(x, y, True)
update_img(img, x, y)
cameo.cmd('led_on')

update_img(img, x, y)
cv2.waitKey(1)
cameo.cmd('speed 1')

while True:
    cameo.serialConnection.reset_input_buffer()
    t0 = time.time()
    cameo.goto_x(-22)
    t1 = t0 + 31.132245540618896
    while time.time() < t1:
        p = (time.time()-t0) / (t1-t0)
        x = -12 - p*10
        update_img(img, x, y)
        cv2.waitKey(10)

    update_img(img, x, y)
    cv2.waitKey(5000)

    cameo.serialConnection.reset_input_buffer()
    t0 = time.time()
    cameo.goto_x(-34)
    t1 = t0 + (41.08889675140381 + 41.09040331840515) / 2
    while time.time() < t1:
        p = (time.time()-t0) / (t1-t0)
        x = -22 - p*12
        update_img(img, x, y)
        cv2.waitKey(10)

    update_img(img, x, y)
    cv2.waitKey(5000)

    cameo.serialConnection.reset_input_buffer()
    t0 = time.time()
    cameo.goto_x(-22)
    t1 = t0 + (41.08889675140381 + 41.09040331840515) / 2
    while time.time() < t1:
        p = (time.time() - t0) / (t1 - t0)
        x = -34 + p * 12
        update_img(img, x, y)
        cv2.waitKey(10)

    update_img(img, x, y)
    cv2.waitKey(5000)

    cameo.serialConnection.reset_input_buffer()
    t0 = time.time()
    cameo.goto_x(-12)
    t1 = t0 + 34.952110290527344
    while time.time() < t1:
        p = (time.time() - t0) / (t1 - t0)
        x = -22 + p * 10
        update_img(img, x, y)
        cv2.waitKey(10)

    update_img(img, x, y)
    cv2.waitKey(5000)
