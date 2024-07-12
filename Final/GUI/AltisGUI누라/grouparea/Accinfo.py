

from PyQt5.QtWidgets import * 

class Accinfo():
    

    def __init__(self):

        self.groupbox = QGroupBox('Acceleration')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        
        self.accXlabel = QLabel('')
        self.accYlabel = QLabel('')
        self.accZlabel = QLabel('')
        
        vbox = QVBoxLayout()

        hbox_AccX = QHBoxLayout()
        hbox_AccX.addWidget(QLabel('X'))
        hbox_AccX.addWidget(self.accXlabel)
        hbox_AccX.addStretch()
        hbox_AccX.addWidget(QLabel('(m/s²)'))

        hbox_AccY = QHBoxLayout()
        hbox_AccY.addWidget(QLabel('Y'))
        hbox_AccY.addWidget(self.accYlabel)
        hbox_AccY.addStretch()
        hbox_AccY.addWidget(QLabel('(m/s²)'))

        hbox_AccZ = QHBoxLayout()
        hbox_AccZ.addWidget(QLabel('Z'))
        hbox_AccZ.addWidget(self.accZlabel)
        hbox_AccZ.addStretch()
        hbox_AccZ.addWidget(QLabel('(m/s²)'))


        vbox.addStretch(1)
        vbox.addLayout(hbox_AccX)
        vbox.addLayout(hbox_AccY)
        vbox.addLayout(hbox_AccZ)
        vbox.addStretch(1)
        self.groupbox.setLayout(vbox)


    # QLabel의 텍스트 업데이트
    def update_accX(self,value):
        self.accXlabel.setText(f"{value:.2f}")

    def update_accY(self,value):
        self.accYlabel.setText(f"{value:.2f}")

    def update_accZ(self,value):
        self.accZlabel.setText(f"{value:.2f}")
