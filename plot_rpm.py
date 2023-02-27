
# importing the required module
import pandas as pds
import matplotlib.pyplot as plt


fileName = "COM3_2023_02_26.23.32.20.687.txt"


dataFrame = pds.read_csv(fileName)
dataFrame.plot(x='Time (s)', y='RPM', kind='line')
plt.ylabel('Revolutions Per Minute')
plt.show()
