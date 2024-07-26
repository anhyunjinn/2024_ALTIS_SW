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
from grouparea.Readybutton import *
from grouparea.Hz import *


baudrate = 115200 # 보드레이트 


class guiMain(QMainWindow):

    def __init__(self):
        super().__init__()
        self.comboBox = QComboBox()
        self.dataBase = DataBase()  # DataBase 객체 인스턴스화
        self.connect_btn = QPushButton('Connect')
        self.com = Communication(baudrate, self.comboBox, self.connect_btn)  # Communication 객체 인스턴스화
        self.Hz = HzButton()  # HzButton 인스턴스 생성

        self.initUI()
        time.sleep(1)


    # UI 초기화
    def initUI(self):
        self.setWindowTitle('ALTIS GUI')  # 이름
        self.setWindowIcon(QIcon('.vscode/windowicon.jpg'))  # 아이콘
        self.setGeometry(0, 0, 2500, 1500)

        # 맨 하단의 상태바 날짜 및 시간
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.updateStatusBar)
        self.timer.start(200)  # Update every second ( 상태바 부분만 해당 )

        # main 레이아웃
        mainLayout = QVBoxLayout()
        vboxacc = QVBoxLayout()
        hboxgraph = QHBoxLayout()
        vboxgraphics = QVBoxLayout()
        vboxright = QVBoxLayout()
        hbox = QHBoxLayout()
        
        # 그래프 레이아웃
        accX_lay = self.setAccXGraph()
        accY_lay = self.setAccYGraph()
        accZ_lay = self.setAccZGraph()

        # 시각화, 고도, button 레이아웃
        visual3D_lay = self.Visual3DMotion()  # 3D 시각화 레이아웃 추가
        groupbutton_lay = self.setGroupbutton()
        rocketaltitude_lay = self.setRocketAltitude()

        # 레이아웃 배치
        vboxacc.addLayout(accX_lay, 2)
        vboxacc.addLayout(accY_lay, 2)
        vboxacc.addLayout(accZ_lay, 2)

        hboxgraph.addLayout(vboxacc, 3)
        hboxgraph.addLayout(rocketaltitude_lay, 1)

        vboxgraphics.addLayout(visual3D_lay, 4)
        vboxgraphics.addSpacing(2)

        vboxright.addLayout(vboxgraphics, 4)

        hbox.addLayout(hboxgraph, 4)
        hbox.addLayout(groupbutton_lay, 4)
        hbox.addLayout(vboxright, 4)

        mainLayout.addLayout(hbox)

        centralWidget = QWidget()
        centralWidget.setLayout(mainLayout)
        self.setCentralWidget(centralWidget)

        self.showMaximized()  # 최대화 화면 설정


    # 맨 하단의 상태바 날짜 및 시간
    def updateStatusBar(self):
        current_date_time = QDateTime.currentDateTime().toString('yyyy-MM-dd HH:mm:ss')
        self.statusBar().showMessage(current_date_time)


    # 가속도 그래프 (X, Y, Z)
    def setAccXGraph(self):
        self.accX = graph_AccX()  # graph_AccX 객체 인스턴스화

        vbox = QHBoxLayout()
        vbox.addWidget(self.accX)
        return vbox

    def setAccYGraph(self):
        self.accY = graph_AccY()  # graph_AccY 객체 인스턴스화

        vbox = QHBoxLayout()
        vbox.addWidget(self.accY)
        return vbox

    def setAccZGraph(self):
        self.accZ = graph_AccZ()  # graph_AccZ 객체 인스턴스화

        vbox = QHBoxLayout()
        vbox.addWidget(self.accZ)
        return vbox

    
    # 고도 시각화
    def setRocketAltitude(self):
        self.rocketAltitude = Rocketaltitude()  # Rocketaltitude 객체 인스턴스화

        vbox = QHBoxLayout()
        vbox.addWidget(self.rocketAltitude)
        return vbox


    # 3D 시각화 로켓 자세 
    def Visual3DMotion(self):
        self.visual3D = Visual3D(self)  # Visual3D 객체 인스턴스화

        vbox = QHBoxLayout()
        vbox.addWidget(self.visual3D)
        return vbox
    
    
    # GroupButton 버튼 영역 및 데이터 수치값 영역 
    def setGroupbutton(self):
        self.AngleInfoGroup = angleinformation()  # angleinformation 객체 인스턴스화
        self.AccelGroup = Accinfo()  # Accinfo 객체 인스턴스화
        self.comSearchGroup = COMSearchGroup(self.com, self.comboBox, self.connect_btn)  # COMSearchGroup 객체 인스턴스화
        self.altitudeInfoGroup = Altitudeinformation()  # Altitudeinformation 객체 인스턴스화
        self.startsaveGroup = StartSaveGroup()  # StartSaveGroup 객체 인스턴스화
        self.EjectionPushButtonGroup = ejectionbutton()  # ejectionbutton 객체 인스턴스화
        self.readyGroup = Readybutton()  # Readybutton 객체 인스턴스화

        hbox = QHBoxLayout()
        vbox1 = QVBoxLayout()
        vbox2 = QVBoxLayout()

        vbox1.addWidget(self.comSearchGroup.groupbox, 1)  # COM 포트 연결 및 찾기
        vbox1.addWidget(self.startsaveGroup.groupbox, 1)  # Save / Start
        vbox1.addWidget(self.AccelGroup.groupbox, 1)  # Acceleration 표시
        vbox1.addWidget(self.altitudeInfoGroup.groupbox, 1)  # 고도, Time 정보
        vbox1.addStretch(1)

        vbox2.addWidget(self.readyGroup.groupbox, 1)  # Ready Button
        vbox2.addWidget(self.EjectionPushButtonGroup.groupbox, 1)  # 사출 버튼
        vbox2.addWidget(self.AngleInfoGroup.groupbox, 1)  # 로켓의 각도 표시
        vbox2.addWidget(self.Hz.groupbox, 1)  # Hz 표시
        vbox2.addStretch(1)

        hbox.addLayout(vbox1, 1)
        hbox.addLayout(vbox2, 1)
           
        self.readyGroup.readybutton.clicked.connect(lambda: self.readyGroup.changecomm(self.com))
        
        self.EjectionPushButtonGroup.eject_button.clicked.connect(lambda: self.EjectionPushButtonGroup.changeEjected(self.com))
        self.EjectionPushButtonGroup.eject_load.clicked.connect(lambda: self.EjectionPushButtonGroup.ejectionReady(self.com))
                
        self.startsaveGroup.start_btn.clicked.connect(self.dataBase.startClicked)
        self.startsaveGroup.start_btn.clicked.connect(lambda: self.EjectionPushButtonGroup.eject_button.setEnabled(True))
        self.startsaveGroup.start_btn.clicked.connect(lambda: self.connect_btn.setDisabled(True))

        self.startsaveGroup.stop_btn.clicked.connect(self.dataBase.stopClicked)
        self.startsaveGroup.stop_btn.clicked.connect(lambda: self.EjectionPushButtonGroup.eject_button.setDisabled(True))
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

    [   0       1    2     3      4        5       6        7           8         9 ]
        time, Accx, Accy, Accz, Anglex, Angley, Anglez, Altitude, EjectionState, Date]
    '''

    def drawGraphs(self, data_table):
        self.accX.update(data_table[1])
        self.accY.update(data_table[2])
        self.accZ.update(data_table[3])
        self.rocketAltitude.updateAltitude(data_table[7])


    def drawGraphic(self, data_table):
        angletable = [float(data_table[4]), float(data_table[5]), float(data_table[6])]
        self.visual3D.updateAngles(angletable)
        self.AngleInfoGroup.update_angleinfo(angletable)


    def drawtext(self, data_table):
        self.AccelGroup.update_accX(float(data_table[1]))  # x Acc
        self.AccelGroup.update_accY(float(data_table[2]))  # y Acc
        self.AccelGroup.update_accZ(float(data_table[3]))  # z Acc
        self.altitudeInfoGroup.update_time(float(data_table[0]) / (1000 * 60))  # time
        self.altitudeInfoGroup.update_altitude(float(data_table[7]))  # altitude
        self.Hz.incrementCount()  # 데이터가 들어올 때마다 카운트 증가
        self.Hz.updateHz()  # HzButton 업데이트


    def drawAll(self):
        if self.com.isOpen():
            try:
                data_table = self.com.getData()
                print(data_table)
                if len(data_table) == 10:
                    self.drawGraphic(data_table)
                    self.drawGraphs(data_table)
                    self.drawtext(data_table)
                    
            except Exception as e:
                print(f"Exception: {e}")   
                
                
app = QApplication(sys.argv)
app.setStyleSheet(qdarkstyle.load_stylesheet(qt_api='pyqt5'))  # 테마 설정

# 폰트 설정
fontDB = QFontDatabase()
fontDB.addApplicationFont('Font/arial.ttf')
app.setFont(QFont('arial'))

ex = guiMain()
timer1 = QTimer()
timer1.start(20)
timer1.timeout.connect(ex.drawAll)

if __name__ == '__main__':
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtWidgets.QApplication.instance().exec_()
        ex.com.close()