
from typing import *
from PyQt5.QtWidgets import *
from SerialCommincation import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *



class Readybutton():
    

    def __init__(self):

        self.groupbox = QGroupBox('Ready Buttons')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        self.comstartbutton = QPushButton('Ready')


        vbox = QVBoxLayout()
        vbox.addStretch(1)
        vbox.addWidget(self.comstartbutton)
        vbox.addStretch(1)
        self.groupbox.setLayout(vbox)
        
    
    def changecomm (self, com : Communication):
        self.getComport(com)
        self.sendCommandstart(self.com)

    def sendCommandstart (self, com : Communication ) :
        if com and com.ser and com.ser.is_open:
            print("command start")
            com.ser.write(f"r\n".encode()) # 통신 명령 r 
            print("command finish")
    


    def getComport (self, com : Communication):
        self.com = com

