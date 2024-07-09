import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from pyqtgraph.Qt import QtCore, QtWidgets, QtGui
import qdarkstyle
import time

from visual3D import *
from graph.AccX import *
from graph.AccY import *
from graph.AccZ import *
from SerialCommincation import *
from grouparea.ejectionbutton import *
from grouparea.Altitudeinformation import *
from grouparea.angleinfomation import * 
from graph.rocketaltitude import *
from grouparea.Accinfo import *
from DataBase.DataBase import *
from grouparea.StartSaveGroup import *
from grouparea.COMSearchGroup import *




baudrate = 115200 # 보드레이트 

class guiMain(QMainWindow):

    def __init__(self):
        super().__init__()
        self.comboBox = QComboBox()
        self.dataBase = DataBase()
        self.connect_btn = QPushButton('Connect')
        self.com = Communication(baudrate, self.comboBox, self.connect_btn)
        self.initUI()
        time.sleep(1)




    def initUI(self):
        self.setWindowTitle('ALTIS GUI') # 이름 
        self.setWindowIcon(QIcon('.vscode/windowicon.jpg')) # 아이콘 
        self.setGeometry(0, 0, 2500, 1500)

        # 맨 하단의 상태바 날짜 및 시간 
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.updateStatusBar)
        self.timer.start(200)  # Update every second ( 상태바 부분만 해당 )


        # 레이아웃 배치 
        mainLayout = QVBoxLayout()
        vboxacc = QVBoxLayout()
        hboxgraph = QHBoxLayout()
        vboxgraphics = QVBoxLayout()

        vboxright = QVBoxLayout() 
        hbox = QHBoxLayout()
        

        accX_lay = self.setAccXGraph()
        accY_lay = self.setAccYGraph()
        accZ_lay = self.setAccZGraph()


        visual3D_lay = self.Visual3DMotion()
        groupbutton_lay = self.setGroupbutton()

        rocketaltitude_lay = self.setRocketAltitude()


        vboxacc.addLayout(accX_lay,2)
        vboxacc.addLayout(accY_lay,2)
        vboxacc.addLayout(accZ_lay,2)

        hboxgraph.addLayout(vboxacc,3)
        hboxgraph.addLayout(rocketaltitude_lay,1)

        vboxgraphics.addLayout(visual3D_lay,4)
        vboxgraphics.addSpacing(2)


        vboxright.addLayout(vboxgraphics,4)

        hbox.addLayout(hboxgraph,4)
        hbox.addLayout(groupbutton_lay, 4)
        hbox.addLayout(vboxright, 4)

        mainLayout.addLayout(hbox)


        centralWidget = QWidget()
        centralWidget.setLayout(mainLayout)
        self.setCentralWidget(centralWidget)

        self.showMaximized() # 최대화 화면 설정 


    #상태창 ( 시간 날짜 )
    def updateStatusBar(self):
        current_date_time = QDateTime.currentDateTime().toString('yyyy-MM-dd HH:mm:ss')
        self.statusBar().showMessage(current_date_time)


    # accX 그래프 
    def setAccXGraph(self):
        self.accX= graph_AccX()

        vbox = QHBoxLayout()
        vbox.addWidget(self.accX)
        return vbox
    
    # accY 그래프 
    def setAccYGraph(self):
        self.accY= graph_AccY()

        vbox = QHBoxLayout()
        vbox.addWidget(self.accY)
        return vbox
    
    # accZ 그래프 
    def setAccZGraph(self):
        self.accZ= graph_AccZ()

        vbox = QHBoxLayout()
        vbox.addWidget(self.accZ)
        return vbox


    # 로켓 고도 
    def setRocketAltitude(self):
        self.rocketAltitude = Rocketaltitude()
        
        vbox = QHBoxLayout()
        vbox.addWidget(self.rocketAltitude)
        return vbox

    #3D 시각화 로켓 자세 
    def Visual3DMotion(self):
        self.visual3D = Visual3D(self)
        
        vbox = QHBoxLayout()
        vbox.addWidget(self.visual3D)
        return vbox
    

    # GroupButton 버튼 영역 및 데이터 수치값 영역 
    def setGroupbutton(self):
        
        self.AngleInfoGroup = angleinformation()
        self.AccelGroup = Accinfo()
        self.comSearchGroup = COMSearchGroup(self.com, self.comboBox, self.connect_btn)
        self.altitudeInfoGroup = Altitudeinformation()
        self.startsaveGroup = StartSaveGroup()
        self.EjectionPushButtonGroup = ejectionbutton()
        
        hbox = QHBoxLayout()
        vbox1 = QVBoxLayout()
        vbox2 = QVBoxLayout()

        vbox1.addWidget(self.comSearchGroup.groupbox,1) # COM 포트 연결 및 찾기
        vbox1.addWidget(self.AccelGroup.groupbox,1) # Acceleration 표시
        vbox1.addWidget(self.AngleInfoGroup.groupbox,1) # 로켓의 각이 얼마큼 변화했는지 각도를 표현 ( x, y, z )
        vbox1.addStretch(2)

        vbox2.addWidget(self.EjectionPushButtonGroup.groupbox,1) # 사출 버튼 
        vbox2.addWidget(self.startsaveGroup.groupbox,1) # Save / Start
        vbox2.addWidget(self.altitudeInfoGroup.groupbox,1) # 고도, Time 정보
        vbox2.addStretch(2)

        hbox.addLayout(vbox1,1)
        hbox.addLayout(vbox2,1)


        # 메서드 정리 & 이벤트 발생
        self.comSearchGroup.connect_btn.clicked.connect(lambda: self.startsaveGroup.start_btn.setEnabled(True))
        
        self.EjectionPushButtonGroup.pushbutton.clicked.connect(lambda: self.EjectionPushButtonGroup.changeEjected(self.com))
        self.EjectionPushButtonGroup.comstartbutton.clicked.connect(lambda: self.EjectionPushButtonGroup.changecomm(self.com))
        self.startsaveGroup.start_btn.clicked.connect(self.dataBase.startClicked)
        self.startsaveGroup.start_btn.clicked.connect(lambda: self.EjectionPushButtonGroup.pushbutton.setEnabled(True))
        self.startsaveGroup.start_btn.clicked.connect(lambda: self.connect_btn.setDisabled(True))

        self.startsaveGroup.stop_btn.clicked.connect(self.dataBase.stopClicked)
        self.startsaveGroup.stop_btn.clicked.connect(lambda: self.EjectionPushButtonGroup.pushbutton.setDisabled(True))
        self.startsaveGroup.stop_btn.clicked.connect(lambda: self.connect_btn.setEnabled(True))

        self.startsaveGroup.start_btn.clicked.connect(lambda: self.accX.startzero())
        self.startsaveGroup.start_btn.clicked.connect(lambda: self.accY.startzero())
        self.startsaveGroup.start_btn.clicked.connect(lambda: self.accZ.startzero())

        self.startsaveGroup.stop_btn.clicked.connect(lambda: self.accX.startzero())
        self.startsaveGroup.stop_btn.clicked.connect(lambda: self.accY.startzero())
        self.startsaveGroup.stop_btn.clicked.connect(lambda: self.accZ.startzero())


        return hbox




    # 데이터 실시간 업데이트 --> 그래프 그리기  
    '''
    data table 

    [    0      1      2     3     4       5       6        7            8         9 ]
    [   time , Accx, Accy, Accz, Anglex, Angley, Anglez, Altitude, EjectionState, Date]
    
          
    리스트 안에 있는 데이터 타입은 문자열, 실수형으로 바꿀려면 float로 바꿔주어야한다. 
    
    '''
    def drawGraphic(self):
        if self.com.isOpen():
            try:
                data_table = self.com.getData()
                
                if len(data_table) == 10:
                    angletable = [float(data_table[4]),float(data_table[5]),float(data_table[6])]
                    self.visual3D.updateAngles(angletable)
                    self.AngleInfoGroup.update_angleinfo(angletable)
            except ValueError  :
                pass
                    
    

    def drawGraphs(self):

        if self.com.isOpen():
            try:
                data_table = self.com.getData()
                #print(float_data_table)
                #print(data_table)
                
                if len(data_table) == 10:
                    print(data_table)# 테이블 길이 
                     # 오류 찾는 테스트 코드: 값이 잘 나오는 지 확인 
                    #print(float_data_table)

                    # 그래프 / 로켓 ( 시각화에 필요한 정보 )
                    # angletable = [float(data_table[4]),float(data_table[5]),float(data_table[6])]

                    self.accX.update(data_table[1])
                    self.accY.update(data_table[2])
                    self.accZ.update(data_table[3])
                    # self.visual3D.updateAngles(angletable)
                    self.rocketAltitude.updateAltitude(data_table[7])
                    
                    # Group Area 데이터 ( 수치값 )
                    # self.AngleInfoGroup.update_angleinfo(angletable) # angle 
                    self.AccelGroup.update_accX(float(data_table[1])) # x Acc
                    self.AccelGroup.update_accY(float(data_table[2])) # y Acc
                    self.AccelGroup.update_accZ(float(data_table[3])) # z Acc
                    self.altitudeInfoGroup.update_time(float(data_table[0])/(1000 * 60)) # time 
                    self.altitudeInfoGroup.update_altitude(float(data_table[7])) # altitude
                    # self.EjectionPushButtonGroup.ledlayout(float(data_table[8]))
                    self.dataBase.saveStart(data_table) # Start / Save 

                    

            except ValueError:
                pass







app = QApplication(sys.argv)
app.setStyleSheet(qdarkstyle.load_stylesheet(qt_api='pyqt5')) # 테마 설정 


# 폰트 설정 
fontDB = QFontDatabase()
fontDB.addApplicationFont('Font\arial.ttf')
app.setFont(QFont('arial'))


ex = guiMain()
timer1 = pg.QtCore.QTimer()
timer2 = pg.QtCore.QTimer()
timer1.start(60)
timer2.start(60)
timer1.timeout.connect(ex.drawGraphs)
timer2.timeout.connect(ex.drawGraphic)

if __name__ == '__main__':
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtWidgets.QApplication.instance().exec_()
        ex.com.close()