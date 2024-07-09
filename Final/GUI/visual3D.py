import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QOpenGLWidget, QVBoxLayout, QWidget, QPushButton, QLabel
from PyQt5.QtCore import QTimer
from OpenGL.GL import *
from OpenGL.GLU import *
import serial
from datetime import datetime
from SerialCommincation import *


class Visual3D(QOpenGLWidget):
    def __init__(self, parent=None):
        super(Visual3D, self).__init__(parent)
        self.xangle, self.yangle, self.zangle = 0, 0, 0
        # self.timer = QTimer(self)
        # self.timer.timeout.connect(self.updateAngles)
        # self.timer.start(50)  # 약 30fps로 업데이트

    def initializeGL(self):
        glEnable(GL_DEPTH_TEST)
        glClearColor(0.1, 0.1, 0.1, 1.0)
        gluPerspective(45, 800/600, 0.1, 50.0)
        glTranslatef(0.0, 0.0, -5)

    def resizeGL(self, width, height):
        glViewport(0, 0, width, height)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45, width / height, 0.1, 50.0)
        glMatrixMode(GL_MODELVIEW)

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        # 오른쪽에서 로켓을 바라보는 카메라 설정
        gluLookAt(5, 0, 0, 0, 0, 0, 0, 0, 1)

        glPushMatrix()
        glRotatef(self.xangle, 1, 0, 0)
        glRotatef(self.yangle, 0, 1, 0)
        glRotatef(self.zangle, 0, 0, 1)
        self.drawRocket()
        glPopMatrix()

    def drawRocket(self):
        body_length = 2.0
        body_radius = 0.2
        cone_height = 0.6
        cone_radius = 0.2

        # Quadric 객체를 한 번만 생성
        quadric = gluNewQuadric()

        # 로켓 몸체 (원기둥)
        glPushMatrix()
        glTranslatef(0, 0, -body_length / 2)
        glColor3f(0.8, 0.8, 0.8)  # Light gray
        gluCylinder(quadric, body_radius, body_radius, body_length, 32, 32)
        glPopMatrix()

        # 로켓 머리 (원뿔)
        glPushMatrix()
        glTranslatef(0, 0, body_length / 2)
        glColor3f(1.0, 0.0, 0.0)  # Red
        gluCylinder(quadric, cone_radius, 0.0, cone_height, 32, 32)
        glPopMatrix()

        # 로켓 꼬리 (디스크)
        glPushMatrix()
        glTranslatef(0, 0, -body_length / 2)
        glRotatef(180, 1, 0, 0)
        glColor3f(0.0, 0.0, 1.0)  # Blue
        gluDisk(quadric, 0.0, body_radius, 32, 32)
        glPopMatrix()

    def updateAngles(self, table):
        # if self.com.isOpen():
        #     try:
        #         data_table = self.com.getData()
        #         if len(data_table) == 4:
        #             self.xangle = float(data_table[1])
        #             self.yangle = float(data_table[2])
        #             self.zangle = float(data_table[3])
        #             self.update()  # 새 각도로 다시 그리기
        #     except ValueError:
        #         pass  # Serial 통신 오류를 처리
        self.xangle = table[0]
        self.yangle = table[1]
        self.zangle = table[2]

