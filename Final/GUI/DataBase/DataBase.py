import numpy as np
import pandas as pd
from datetime import datetime

"""

    data table 

    [   0        1        2        3         4        5       6         7              8          , 9    ]
        time ,  Accx,    Accy,    Accz,    Anglex,  Angley,  Anglez,  Altitude,     EjectionState, Date  ]
    
          
    리스트 안에 있는 데이터 타입은 문자열, 실수형으로 바꿀려면 float로 바꿔주어야한다. 
    
"""

class DataBase():
    columns = ["Time", "xAcc", "yAcc", "zAcc","xAngle", "yAngle", "zAngle","Altitude","EjectionState" ,"Date" ]
    def __init__(self):
        self.df = pd.DataFrame(columns=self.columns)
        self.isSaving = False

    def startClicked(self):
        self.isSaving = True
        

    def stopClicked(self):
        self.isSaving = False
        exceldataName = "C:\\Users\\jully\\Downloads\\2024_ALTIS_SW\\통신\\GUI\\AltisGUI누라\\Data\\Rocket_Data_" + self.dateTime + ".xlsx"
        try:
            self.df.to_excel(exceldataName)
        except Exception as e:
            print(f"파일 저장 실패: {e}")

    def saveStart(self, data_table:list):
        # if (self.isSaving):
        data_table[8] = self.isEjectYN(data_table[8])
        self.dateTime = datetime.now().strftime("%Y%m%d_%H%M%S")
        self.df.loc[len(self.df)] = data_table


    def isEjectYN(self, isEject):
        if (isEject == '1'):   # OKD 코드 보고 값 고쳐야함
            return "Ejected"

        else:
            return np.NaN