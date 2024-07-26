from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
import time
from SerialCommincation import *

# class HzButton():
#     def __init__(self):
#         self.groupbox = QGroupBox('Hz Display')
#         self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        

#         self.hz_label = QLabel('Frequency:')
#         self.hz_display = QLCDNumber()
#         self.hz_display.setDigitCount(6)

#         self.count = 0
#         self.start_time = time.time()
        
#         layout = QVBoxLayout()
#         layout.addWidget(self.hz_label)
#         layout.addWidget(self.hz_display)

#         self.groupbox.setLayout(layout)

#         self.timer = QTimer()
#         self.timer.timeout.connect(self.updateHz)
#         self.timer.start(1000)  # Update every second

#     def incrementCount(self):
#         self.count += 1

#     def updateHz(self):
#         elapsed_time = time.time() - self.start_time
#         hz = self.count / elapsed_time if elapsed_time > 0 else 0
#         self.hz_display.display(hz)
#         self.count = 0
#         self.start_time = time.time()


class HzButton():
    def __init__(self):
        self.groupbox = QGroupBox('Hz Display')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        
        self.hz_label = QLabel('Frequency:')
        self.hz_value = QLabel('0')

        layout = QVBoxLayout()
        layout.addStretch(1)
        layout.addWidget(self.hz_label)
        layout.addWidget(self.hz_value)
        layout.addStretch(1)

        self.groupbox.setLayout(layout)

        self.timer = QTimer()
        self.timer.timeout.connect(self.updateHz)
        self.timer.start(1000)  # Update every second

        self.count = 0
        self.start_time = time.time()

    def incrementCount(self):
        self.count += 1

    def updateHz(self):
        elapsed_time = time.time() - self.start_time
        hz = self.count / elapsed_time if elapsed_time > 0 else 0
        self.hz_value.setText(f'{hz:.2f}')
        self.count = 0
        self.start_time = time.time()

# Example usage
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)

    hz_button = HzButton()

    window = QWidget()
    layout = QVBoxLayout()
    layout.addWidget(hz_button.groupbox)
    window.setLayout(layout)
    window.show()

    sys.exit(app.exec_())