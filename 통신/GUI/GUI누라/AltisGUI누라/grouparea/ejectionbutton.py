
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


        self.ledlayout = QHBoxLayout()
        self.ledlayout.addWidget(CircleWidget(64, 15, 7))
        self.ledlayout.addWidget(CircleWidget(3, 46, 8))
        self.ledlayout.addStretch()


        vbox = QVBoxLayout()
        vbox.addStretch(1)
        vbox.addLayout(self.ledlayout)
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
            com.ser.write(f"r\n".encode()) # 사출 명령 r 


    def getComport (self, com : Communication):
        self.com = com


    def stateEJ(self, EjectionState):
        hbox_led = QHBoxLayout()
        if EjectionState == 0 : 

            hbox_led.addWidget(CircleWidget(250, 75, 62))
            hbox_led.addWidget(CircleWidget(3, 46, 8))

        elif EjectionState == 1 :  
        
            hbox_led.addWidget(CircleWidget(64, 15, 7))
            hbox_led.addWidget(CircleWidget(0, 255, 30))

        else : 
            hbox_led.addWidget(CircleWidget(64, 15, 7))
            hbox_led.addWidget(CircleWidget(3, 46, 8))
        
        self.ledlayout =  hbox_led
        




class CircleWidget(QWidget):
    def __init__(self, r,g,b, parent=None):
        super().__init__(parent)
        self.color = QColor(r,g,b)
        self.setFixedSize(QSize(40, 40))

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.setBrush(self.color)
        painter.setPen(Qt.NoPen)
        painter.drawEllipse(0, 0, self.width(), self.height())