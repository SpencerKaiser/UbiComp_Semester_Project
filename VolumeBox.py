
from matplotlib import pyplot as plt
from collections import deque
from math import fabs
import serial
import os
import time                             #Used for sleep()


volumes = {}
volumes[0] = 'osascript -e "set Volume 0"'
volumes[1] = 'osascript -e "set Volume 2"'
volumes[2] = 'osascript -e "set Volume 4"'
volumes[3] = 'osascript -e "set Volume 6"'
volumes[4] = 'osascript -e "set Volume 7"'

osVolume = {}
osVolume[0] = 0
osVolume[1] = 2
osVolume[2] = 4
osVolume[3] = 6
osVolume[4] = 7

curVol = 2
muted = False

serial_port = serial.Serial('/dev/tty.usbmodem1451', 9600, timeout=1)
serial_port.flush()

while True:
    rawVal = serial_port.read()
    if rawVal:
        val = ord(rawVal)
        if val > 10:
            curVol = val - 10
            os.system(volumes[curVol])
            mute = False
            print("Volume Changed to " + str(osVolume[curVol]))
        else:
            if val is 0:
                if muted:
                    os.system(volumes[curVol])
                    print("Volume Restored [" + str(osVolume[curVol]) + "]")
                    muted = False
                else:
                    os.system(volumes[0])
                    muted = True
                    print("Mute Toggled")
            elif val is 1:
                print("Playing Previous Track")
                os.system("open previousTrack.app")
            elif val is 2:
                os.system("open PausePlay.app")
                print("Pause/Play")
            elif val is 3:
                os.system("open nextTrack.app")
                print("Playing Next Track")


serial_port.close()

