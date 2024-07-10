
from PyQt5.QtWidgets import *
from SerialCommincation import *


class COMSearchGroup():
    def __init__(self, com : Communication , combo : QComboBox , btn : QPushButton):
        self.com = com
        self.comboBox = combo
        self.connect_btn = btn

        self.groupbox = QGroupBox('Port Connected')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        vbox = QVBoxLayout()

        cb = self.comboBox
        cb.addItem('None')

        self.search_btn = QPushButton('Search')

        self.search_btn.clicked.connect(self.com.searchPort)
        self.connect_btn.clicked.connect(self.com.connectPort)
        
        vbox.addStretch(1)
        vbox.addWidget(self.search_btn)
        vbox.addWidget(self.connect_btn)
        vbox.addWidget(cb)
        vbox.addStretch(1)

        self.groupbox.setLayout(vbox)

