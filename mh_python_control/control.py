from Cameo25 import Cameo25
import pygame
import time


#cameo = Cameo25(115200, debug=True)

# Get count of joysticks
pygame.init()
pygame.joystick.init()

joystick = pygame.joystick.Joystick(0)  # take the first one
joystick.init()

old_e = None
while True:
    time.sleep(1/60)  # 60 hertz refresh rate
    events = pygame.event.get()

    if len(events) != 0:
        # events are waiting in the queue

        last_x = None
        last_y = None

        # we get the latest of the events from the queue
        for e in events:
            if e.type == 7:  # 7 is JOYAXISMOTION
                if e.__dict__["axis"] == 0:  # x axis
                    last_x = e
                if e.__dict__["axis"] == 1:  # y axis
                    last_y = e

        if last_x is not None:
            print("x: " + str(last_x.__dict__["value"]))

        if last_y is not None:
            print("y: " + str(last_y.__dict__["value"]))

# # For each joystick:
# for i in range(joystick_count):
#     joystick = pygame.joystick.Joystick(i)
#     joystick.init()

#cameo.stop_all()

#cameo.die()

pygame.quit()

#cameo.reset_die()
