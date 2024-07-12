
from typing import *
from PyQt5.QtWidgets import *
from SerialCommincation import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *



class ejectionbutton():
    

    def __init__(self):

        self.groupbox = QGroupBox('Ejection Buttons')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        self.pushbutton = QPushButton('Eject')
        self.pushbutton.setDisabled(True)


        vbox = QVBoxLayout()
        vbox.addStretch(1)
        vbox.addWidget(self.pushbutton)
        vbox.addStretch(1)
        self.groupbox.setLayout(vbox)


    def changeEjected (self, com : Communication):
        self.pushbutton.setText('Ejected')
        self.getComport(com)
        self.pushbutton.setDisabled(True)
        self.sendCommand(self.com)
        

    
    def sendCommand (self, com : Communication ) :
        if com and com.ser and com.ser.is_open:
            com.ser.write(f"e\n".encode()) # 사출 명령 e 


    def getComport (self, com : Communication):
        self.com = com


