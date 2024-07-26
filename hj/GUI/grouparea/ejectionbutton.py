from typing import *
from PyQt5.QtWidgets import *
from SerialCommincation import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

class ejectionbutton():
    def __init__(self):
        self.groupbox = QGroupBox('Ejection Buttons')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        self.eject_button = QPushButton('Eject')
        self.eject_load = QPushButton('Load')

        vbox = QVBoxLayout()
        vbox.addStretch(1)
        vbox.addWidget(self.eject_load)
        vbox.addWidget(self.eject_button)
        vbox.addStretch(1)
        self.groupbox.setLayout(vbox)
        
        # self.eject_button.clicked.connect(lambda: self.changeEjected)
        # self.eject_load.clicked.connect(lambda: self.ejectionReady)
    
    def changeEjected (self, com : Communication):
        self.getComport(com)
        self.send_Ejection_Command (self.com)
        
    def ejectionReady (self, com : Communication):
        self.getComport(com)
        self.send_Load_Command(self.com)
    
    def send_Ejection_Command (self, com : Communication ) :
        if com and com.ser and com.ser.is_open:
            com.ser.write(f"eee\n".encode()) # 사출 명령 e 
            
    def send_Load_Command (self, com : Communication ) :
        if com and com.ser and com.ser.is_open:
            com.ser.write(f"lll\n".encode()) # 사출 명령 l 
            
    def getComport (self, com : Communication):
        self.com = com


