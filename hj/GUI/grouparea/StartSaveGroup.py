
from PyQt5.QtWidgets import *
from DataBase.DataBase import *
from PyQt5.QtCore import QTimer, QTime


class StartSaveGroup():
    def __init__(self):
        self.groupbox = QGroupBox('Data Save / Start')
        self.groupbox.setStyleSheet("QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; }")
        vbox = QVBoxLayout()


        self.start_btn = QPushButton('Start')
        self.stop_btn = QPushButton('Stop')
        self.time_label = QLabel('Elapsed Time: 0:00:00')

        vbox.addStretch(1)
        vbox.addWidget(self.start_btn)
        vbox.addWidget(self.stop_btn)
        vbox.addStretch(1)
        vbox.addWidget(self.time_label)
        vbox.addStretch(1)

        self.start_time = None
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_time)

        self.start_btn.clicked.connect(self.buttonstartED)
        self.stop_btn.clicked.connect(self.buttonstopED)

        self.groupbox.setLayout(vbox)
    

    def buttonstartED(self):
        self.start_time = QTime.currentTime()
        self.timer.start(1000)  # Update every second
        self.start_btn.setDisabled(True)
        self.stop_btn.setEnabled(True)


    def buttonstopED(self):
        self.timer.stop()
        self.start_btn.setEnabled(True)
        self.stop_btn.setDisabled(True)


    def update_time(self):
        if self.start_time:
            elapsed_time = self.start_time.elapsed() // 1000  # Get elapsed time in seconds
            hours, remainder = divmod(elapsed_time, 3600)
            minutes, seconds = divmod(remainder, 60)
            self.time_label.setText(f'Elapsed Time: {hours}:{minutes:02}:{seconds:02}')


