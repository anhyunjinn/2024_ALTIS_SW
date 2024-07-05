import pyqtgraph as pg
import numpy as np

class graph_AccZ(pg.PlotWidget):
    def __init__(self):
        super().__init__()
        self.setTitle('Acc Z', color='w')
        self.setLabel('left', 'Acc Z ( m/s²)')
        self.setLabel('bottom', 'Time(s)')

        #graph style
        pen = pg.mkPen(color=(0, 100, 0), width=3)

        #set graph data
        self.x = np.linspace(0,1,20)
        self.y = np.linspace(0,0,20)
        self.plot = self.plot(self.x, self.y, pen=pen)


    
    def update(self, value):
        x = self.x[-1]
        self.x[:-1] = self.x[1:]
        self.x[-1] = x+0.02 #Since update graph every 0.2sec

        self.y[:-1] = self.y[1:] #데이터 테이블 이동
        self.y[-1] = float(value) #마지막에 새로운 데이터 추가

        self.plot.setData(self.x, self.y)

    def startzero(self): # 다시 time 0 부터 실행 
        self.x = np.linspace(0,1,20)
