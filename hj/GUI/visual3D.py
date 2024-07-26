import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QOpenGLWidget
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QSurfaceFormat
from OpenGL.GL import *
from OpenGL.GLU import *

class Visual3D(QOpenGLWidget):
    def __init__(self, parent=None):
        super(Visual3D, self).__init__(parent)
        self.pitch, self.roll, self.yaw = 0, 0, 0

    def initializeGL(self):
        glEnable(GL_DEPTH_TEST)
        glClearColor(0.0, 0.0, 0.0, 1.0)
        gluPerspective(45, (self.width() / self.height()), 0.1, 50.0)
        glTranslatef(0.0, 0.0, -10)

    def resizeGL(self, w, h):
        glViewport(0, 0, w, h)
        gluPerspective(45, (w / h), 0.1, 50.0)

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        gluPerspective(45, (self.width() / self.height()), 0.1, 50.0)
        glTranslatef(0.0, 0.0, -10)
        glRotatef(-90, 1, 0, 0)  # 기본적으로 X축을 기준으로 -90도 회전 (위로 향하게 설정)
        glRotatef(self.pitch, 1, 0, 0)  # pitch 회전 (X축 기준)
        glRotatef(-self.roll, 0, 1, 0)  # roll 회전 (Y축 기준, 부호 반대로)
        self.draw_rocket()
        self.update()

    def draw_rocket(self):
        # 원기둥 그리기
        glPushMatrix()
        glTranslatef(0.0, 0.0, -1.5)  # 원기둥을 중앙으로 이동
        glColor3f(0.5, 0.5, 0.5)  # 원기둥을 회색으로 설정
        gluCylinder(gluNewQuadric(), 0.5, 0.5, 3, 32, 32)
        glPopMatrix()

        # 원뿔 그리기
        glPushMatrix()
        glTranslatef(0.0, 0.0, 1.5)  # 원뿔을 원기둥의 꼭대기로 이동
        glColor3f(1, 0, 0)  # 원뿔을 붉은색으로 설정
        gluCylinder(gluNewQuadric(), 0.5, 0.0, 1.5, 32, 32)
        glPopMatrix()

        # 원기둥의 밑면 그리기
        glPushMatrix()
        glTranslatef(0.0, 0.0, -1.5)  # 원기둥의 밑면 위치로 이동
        glColor3f(0, 0, 1.0)  # 밑면을 파란색으로 설정
        gluDisk(gluNewQuadric(), 0, 0.5, 32, 32)
        glPopMatrix()

    def updateAngles(self, table):
        self.pitch = table[0]
        self.roll = table[1]
        self.yaw = table[2]
        self.update()


