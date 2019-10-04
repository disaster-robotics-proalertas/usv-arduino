#!/bin/python
# Power_monitor
# by Guilherme Heck
# guilherme.heck@acad.pucrs.br
# guilherme.heck@edu.pucrs.br
# Last update: 04/10/2019

import RPi.GPIO as GPIO
import os
import time

gpio_pin_number=17

GPIO.setmode(GPIO.BCM)

GPIO.setup(gpio_pin_number, GPIO.IN, pull_up_down=GPIO.PUD_UP)

try:
    GPIO.wait_for_edge(gpio_pin_number, GPIO.FALLING)
    if not GPIO.input(gpio_pin_number):
       print('GPIO Power Control')
       time.sleep(1)
       if not GPIO.input(gpio_pin_number):
           print('Shutdown')
           os.system("sudo shutdown -h now")
       else:
           print('Reset')
           os.system("sudo reboot")

except:
    pass

GPIO.cleanup()


