# IR2X10
Rasberry pi gpio control via IR remote (using wiring pi and lirc).
Raspberry pi gpio Android server.
The raspberry has an IR diode on PIN GPIO02 and one RF433Mhz transmiter on PIN03.
Receiving RF signals fron a RF remote (Euskaltel brand, but you can use your 
own remote with proper lirc.conf file) you can control two motorized blinds.
conected via X10 modules adrressed on A2 and A3 via ON, OFF and DIM conmands.
You need to install and setup lirc and wiringpi (google about that if you need).
In /boot/config.txt add this line at the end for the lirc IR imput:
dtoverlay=lirc-rpi:gpio_in_pin=2.
You can modify X10 conmands as you wishes.

