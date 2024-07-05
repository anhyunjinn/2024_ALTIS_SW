import sys
from PyQt5.QtWidgets import QApplication, QVBoxLayout, QWidget
import pyqtgraph as pg
from pyqtgraph import PlotWidget

class Rocketaltitude(PlotWidget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.initUI()

    def initUI(self):
       # self.setFixedSize(200, 1575)
        self.getPlotItem().hideAxis('bottom')
        
        # Set up the plot
        self.setYRange(-20, 500)  # 고도 범위 설정
        self.setXRange(0, 0.2)
        
        # Set up the rocket representation (yellow arrow)
        self.arrow = pg.ArrowItem(angle=90, tipAngle=15, baseAngle=20, headLen=40, tailLen=20, tailWidth=10, pen={'color': 'y', 'width': 3}, brush='y')
        self.text = pg.TextItem(text='0.00 m', color='w', anchor=(0, 0.5))
        self.addItem(self.arrow)
        self.addItem(self.text)
        
        # Update the plot
        self.updateAltitude(altitude= 0.0)

    def updateAltitude(self, altitude):
        # Update rocket position
        altitude = float(altitude)
        self.arrow.setPos(0.05, altitude)
        self.text.setPos(0.0, altitude + 20)
        self.text.setText(f"{altitude:.2f} m")


        