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
    columns = ["Time", "xAcc", "yAcc", "zAcc","xAngle", "yAngle", "zAngle","Altitude","EjectionState","Date" ]
    def __init__(self):
        self.df = pd.DataFrame(columns=self.columns)
        self.isSaving = False

    def startClicked(self):
        self.isSaving = True
        

    def stopClicked(self):
        self.isSaving = False
        exceldataName = "Data/Rocket_Data" + self.dateTime + ".xlsx" 
        self.df.to_excel(exceldataName)

    def saveStart(self, data_table:list):
        if (self.isSaving):
            self.dateTime = datetime.now().strftime("%Y%m%d_%H%M%S")
            self.df.loc[len(self.df)] = data_table



    