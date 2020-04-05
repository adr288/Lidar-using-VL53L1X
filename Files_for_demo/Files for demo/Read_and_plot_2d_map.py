import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
from matplotlib.colors import BoundaryNorm
from matplotlib.ticker import MaxNLocator
import numpy as np
import serial #Import Serial Library

import time
from time import sleep

arduinoSerialData = serial.Serial('/dev/ttyUSB0',115200) #Create Serial port object called arduinoSerialData
size_of_the_matrix = 5


x, y = np.meshgrid(np.linspace(0,5,6), np.linspace(0,5,6)) #each axis is from 0 to 3 devided in 4 sections


distances_matrix = np.zeros((size_of_the_matrix,size_of_the_matrix))




fig = plt.figure(figsize=None,facecolor='white')

ax2 = plt.subplot()
quad1 = ax2.pcolormesh(x,y,distances_matrix,vmin=0,vmax=3000,shading='flat')
bar = fig.colorbar(quad1, ax=ax2)
bar.set_label("Ranges in each ROI (mm)")
ax2.set_xlabel('X')
ax2.set_ylabel('Y')



def init():
    quad1.set_array([])
    return quad1

def animate(iter):
    global distances_matrix
    if (arduinoSerialData.inWaiting()>0): #Check if data is on the serial port
        data_receving_from_arduino_as_string= arduinoSerialData.readline()
      
        distances_in_string = data_receving_from_arduino_as_string.split(',') #Convering the data received from the arduino from string to a float
        distances_in_string.pop()
        distances = [float(i) for i in distances_in_string]
        

        if(len(distances)==size_of_the_matrix*size_of_the_matrix):
         

          distances_matrix = np.reshape(distances, (size_of_the_matrix, size_of_the_matrix))
          

          b = distances_matrix[:,0:1]
          #print b
          a = distances_matrix [:,1:5]
          #print a
          c = np.concatenate((a,b), axis =1) 

          distances_matrix = np.flipud(c)
          #distances_matrix = c
          

    #print distances_matrix
    quad1.set_array(distances_matrix.ravel())
    




anim = animation.FuncAnimation(fig,animate ,interval=100,blit=False,repeat=False)
plt.show()

