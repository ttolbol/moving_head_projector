from Cameo25 import Cameo25
import time


cameo = Cameo25(115200, debug=True)

#Ramp up the speed
for i in range(10, 350, 5):
    cameo.move_x(i)
    time.sleep(0.02)

#Ramp down
for i in range(350, 10, -5):
    cameo.move_x(i)
    time.sleep(0.02)

cameo.stop_all()

cameo.die()
