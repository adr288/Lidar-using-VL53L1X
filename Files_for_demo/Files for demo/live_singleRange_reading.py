import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
from matplotlib import style
import serial #Import Serial Library


import time
from time import sleep
arduinoSerialData = serial.Serial('/dev/ttyUSB0',115200) #Create Serial port object called arduinoSerialData


style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)




data_receving_from_arduino_as_string =[]
time = []
distances = []
count = 0


def animate(i):
    
    global count
    
    if (arduinoSerialData.inWaiting()>0): #Check if data is on the serial port
     new_data_as_string = arduinoSerialData.readline()
     new_data_string = new_data_as_string.strip()  
     if (new_data_string !=''):
      new_data = float(new_data_string)
      
      count +=0.1
      distances.append(new_data)
      time.append(count)
      

    #time = range(0,len(count))
    #time = range(0,100)
   
    ax1.clear()
    ax1.plot(time, distances)
    ax1.set(xlabel='Time (seconds)', ylabel='Distance (mm)',
       title='Single distance measured by VL53L1X sensor(update rate: 50 Hz)')
    if (len(distances)>100):
      distances.pop(0)
      time.pop(0)




    
#for animation
ani = animation.FuncAnimation(fig, animate, interval=10)
plt.show()




# while(1):

#  if (arduinoSerialData.inWaiting()>0): #Check if data is on the serial port
#     new_data_as_string = arduinoSerialData.readline()
#     new_data_string = new_data_as_string.strip()
#     if (new_data_string !=''):
#      print new_data_string
#      try:
#       new_data = float(new_data_string)
#      except ValueError,e:
#       print repr(new_data_string)
#       break

#     distances.append(new_data)

   
