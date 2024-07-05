from PyQt5.QtWidgets import *

class Altitudeinformation(QWidget):

    def __init__(self):

        self.groupbox = QGroupBox('Altitude and Time')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        self.time = QLabel('')
        self.altitude = QLabel('')

        vbox = QVBoxLayout()
        
        hbox_alti = QHBoxLayout()
        hbox_alti.addWidget(QLabel('Altitude'))
        hbox_alti.addWidget(self.altitude)
        hbox_alti.addStretch()
        hbox_alti.addWidget(QLabel('(m)'))

        hbox_time = QHBoxLayout()
        hbox_time.addWidget(QLabel('Time'))
        hbox_time.addWidget(self.time)
        hbox_time.addStretch()
        hbox_time.addWidget(QLabel('(s)'))

    
        vbox.addStretch(1)
        vbox.addLayout(hbox_alti)
        vbox.addLayout(hbox_time)
        vbox.addStretch(1)
        self.groupbox.setLayout(vbox)


    # QLabel의 텍스트 업데이트


    def update_altitude(self,value):
        self.altitude.setText(f"{value:.2f}")

    def update_time(self,value):
        self.altitude.setText(f"{value}")


