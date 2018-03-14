from Cameo25 import Cameo25
import time


cameo = Cameo25(115200, debug=True)

cameo.goto_y(90)



#cameo.stop_all()
cameo.reset_die()
