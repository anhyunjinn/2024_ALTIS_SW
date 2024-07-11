

#from email.charset import QP
#from tokenize import String
#import random
import serial
import serial.tools.list_ports
import time
import struct
#import numpy as np

from PyQt5.QtWidgets import QComboBox, QPushButton
from datetime import datetime


class Communication:
    """
    baudrate = ''
    portName = ''
    ports = serial.tools.list_ports.comports()
    """
    ser = serial.Serial()

    def __init__(self, baudrate:int, comboBox:QComboBox, connect_btn:QPushButton):
        self.baudrate = baudrate
        self.cbBox = comboBox
        self.isConnected = 0
        self.connect_btn = connect_btn

    def searchPort(self):
        self.ports = serial.tools.list_ports.comports()
        self.cbBox.clear()
        for port in sorted(self.ports):
            
            portSplit = str(port).split("-")
            self.cbBox.addItem(portSplit[:-1][0])

    def connectPort(self):
        if (self.isConnected == False):
            self.portName = self.cbBox.currentText()

            try:
                self.ser = serial.Serial(self.portName, self.baudrate)
                self.isConnected = 1
                self.connect_btn.setText("Connected")
                time.sleep(1)
            

            except serial.SerialException:
                print("Can't Open : ", self.portName)
                self.connect_btn.setText("Connect")

        else:
            self.close()
            self.isConnected = 0
            self.connect_btn.setText("Connect")


    def close(self):
        if(self.ser.isOpen()):
            self.ser.close()


    # 3434,54,565,565 이런식으로 받아오는 방식
    def getData(self):
        
        if (self.ser.inWaiting() > 0):
            
            value = self.ser.readline()
            #value = self.sio.readline()
    
            decoded = str(value[0:len(value) - 2].decode("utf-8"))
            value_table = decoded.split(",")
            value_table.insert(9, self.getDate())
        else:
            value_table = []
        

        return value_table

    # 구조체 받아오는 거 
    # def getData(self):

    #     if ( self.ser.inWaiting() > 0):

    #         structFormat = 'LfffffffB' # 구조체 포맷 
    #         structSize = struct.calcsize(structFormat)
    #         data = self.ser.read(structSize)
    #         datalist = list(struct.unpack(structFormat, data))
    #         value_table = [str(element).encode('utf-8') for element in datalist]
    #         value_table.insert(0, self.getDate())
    #     else :
    #         value_table = []

    #     return value_table


    def getDate(self):
        now = datetime.now()
        date = now.strftime("%Y-%m-%d %H:%M:%S")
        return date

    
    def isOpen(self):
        return self.ser.isOpen()


