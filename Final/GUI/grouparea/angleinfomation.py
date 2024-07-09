

from PyQt5.QtWidgets import *

class angleinformation():
    

    def __init__(self):

        self.groupbox = QGroupBox('Changed Angle')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        self.info1label = QLabel('')
        self.info2label = QLabel('')
        self.info3label = QLabel('')
        
        vbox = QVBoxLayout()

        hbox_AngleX = QHBoxLayout()
        hbox_AngleX.addWidget(QLabel('AngleX'))
        hbox_AngleX.addWidget(self.info1label)
        hbox_AngleX.addStretch()
        hbox_AngleX.addWidget(QLabel('(°)'))

        hbox_AngleY = QHBoxLayout()
        hbox_AngleY.addWidget(QLabel('AngleY'))
        hbox_AngleY.addWidget(self.info2label)
        hbox_AngleY.addStretch()
        hbox_AngleY.addWidget(QLabel('(°)'))

        hbox_AngleZ = QHBoxLayout()
        hbox_AngleZ.addWidget(QLabel('AngleZ'))
        hbox_AngleZ.addWidget(self.info3label)
        hbox_AngleZ.addStretch()
        hbox_AngleZ.addWidget(QLabel('(°)'))

        vbox.addStretch(1)
        vbox.addLayout(hbox_AngleX)
        vbox.addLayout(hbox_AngleY)
        vbox.addLayout(hbox_AngleZ)
        vbox.addStretch(1)
        self.groupbox.setLayout(vbox)


    # QLabel의 텍스트 업데이트
    def update_angleinfo(self,table):
        self.info1label.setText(str(table[0]))
        self.info2label.setText(str(table[1]))
        self.info3label.setText(str(table[2]))
