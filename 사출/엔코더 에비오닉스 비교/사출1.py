import serial
import time
from datetime import datetime

# 직렬 포트 설정
ser1 = serial.Serial('COM7', 115200)  # 아두이노 1 (에비오닉스)
ser2 = serial.Serial('COM8', 9600)  # 아두이노 2 (엔코더)

while True:

# 로그 파일 이름 생성
    log_file = f"sensor_data_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"

    # 로그 파일 열기
    with open(log_file, 'w') as f:
        while True:
            # 아두이노 1에서 데이터 읽기
            data1 = ser1.readline().decode().strip()
            data2 = ser2.readline().decode().strip()
            # 아두이노 2에서 데이터 읽기
            
            # 데이터 처리 및 로깅
            timestamp = int(time.time() * 1000)
            f.write(f"{timestamp},{data1},{data2}\n")
            print(f"{timestamp},{data1},{data2}\n")
            f.flush()
