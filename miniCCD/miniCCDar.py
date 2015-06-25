#!/bin/env python
"""
osPID_gui.py

based on lpr.py, by Mahesh Venkitachalam, Website: electronut.in

modified 6-17-15 to scroll from the right and parse
setpoint and current-temp data from
an osPID on the named serial port.

AMCDawes

"""

import sys, serial, argparse
import numpy as np
from time import sleep

import matplotlib.pyplot as plt
import matplotlib.animation as animation


# plot class
class AnalogPlot:
  # constr
  def __init__(self, strPort):
      # open serial port
      self.ser = serial.Serial(strPort, 115200)

      self.ax = []

  # add data
  def add(self, data):
      self.ax = data

  # update plot
  def update(self, frameNum, a0):
      try:
          line = self.ser.readline()
          data = []
          #print line
          line = line.split(";")
          data = [float(val) for val in line[:-1]]
          #print data
          self.add(data)
          a0.set_data(range(len(data)), self.ax)
      except KeyboardInterrupt:
          print('exiting')

      return a0,

  # clean up
  def close(self):
      # close serial
      self.ser.flush()
      self.ser.close()

# main() function
def main():
  # create parser
  parser = argparse.ArgumentParser(description="LDR serial")
  # add expected arguments
  parser.add_argument('--port', dest='port', required=True)

  # parse args
  args = parser.parse_args()

  #strPort = '/dev/tty.usbserial-A7006Yqh'
  strPort = args.port

  print('reading from serial port %s...' % strPort)

  # plot parameters
  analogPlot = AnalogPlot(strPort)

  print('plotting data...')

  # set up animation
  fig = plt.figure()
  ax = plt.axes(xlim=(0, 255), ylim=(0, 1024))
  a0, = ax.plot([], [])
  anim = animation.FuncAnimation(fig, analogPlot.update, fargs=(a0,), interval=50)

  # show plot
  plt.show()

  # clean up
  analogPlot.close()

  print('exiting.')


# call main
if __name__ == '__main__':
  main()
