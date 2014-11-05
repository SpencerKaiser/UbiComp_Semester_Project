
from matplotlib import pyplot as plt
from collections import deque
from math import fabs
import serial
import os
import time                             #Used for sleep()


volumes = {}
volumes[0] = 'osascript -e "set Volume 0"'
volumes[1] = 'osascript -e "set Volume 1"'
volumes[2] = 'osascript -e "set Volume 2"'
volumes[3] = 'osascript -e "set Volume 3"'
volumes[4] = 'osascript -e "set Volume 4"'
volumes[5] = 'osascript -e "set Volume 5"'
volumes[6] = 'osascript -e "set Volume 6"'
volumes[7] = 'osascript -e "set Volume 7"'

curVol = 0

serial_port = serial.Serial('/dev/tty.usbmodem1411', 14400, timeout=1)
serial_port.flush()

__location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))
pathFile = open(os.path.join(__location__, 'pathFile.txt'))
scriptsPath = pathFile.readline()


currLevel = 0
val_as_int = 101
prevVal = 101
diff = 0
waiting = True                  #Initialized to true to allow for muting on first wave over sensor
muted = False
buttonCounter = 0

while True:
    val = serial_port.read()
    if val:
        val_as_int = ord(val)

    diff = fabs(val_as_int - prevVal)

    print(val_as_int)
    if val_as_int is 200:
        while val_as_int is 200:
            val = serial_port.read()
            val_as_int = ord(val)
            buttonCounter += 1
        if buttonCounter >= 7:
            os.system('open '+scriptsPath+'NextTrack.app')
            buttonCounter = 0
        else:
            os.system('open '+scriptsPath+'PausePlay.app')
            buttonCounter = 0
    elif diff >= 3 and val_as_int >= 10:
        if 10 <= val_as_int < 17 and curVol is not 2:
            os.system(volumes[2])
            curVol = 2
            print("Volume Changed to 2")
        elif 17 <= val_as_int < 24 and curVol is not 4:
            os.system(volumes[4])
            curVol = 4
            print("Volume Changed to 4")
        elif 24 <= val_as_int < 31 and curVol is not 6:
            os.system(volumes[6])
            curVol = 6
            print("Volume Changed to 6")
        elif 31 <= val_as_int < 34 and curVol is not 7:
            os.system(volumes[7])
            curVol = 7
            print("Volume Changed to MAX")
        prevVal = val_as_int
        if val_as_int > 34:
            waiting = True
        else:
            muted = False
    elif (val_as_int < 10 or val_as_int > 1000) and waiting is True:
        if muted:
            os.system(volumes[curVol])
            muted = False
            print("VOLUME RESTORED")
        elif not muted:
            os.system(volumes[0])
            muted = True
            print("MUTED")
        prevVal = val_as_int
        waiting = False
        # print("Mute Toggled")

serial_port.close()

