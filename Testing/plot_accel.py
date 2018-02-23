#plotter.py
#Copyright Eivind Roson Eide 2018

#use: python plotter.pu out/file.txt [save]

import numpy as np
import matplotlib.pyplot as plt
import sys
import time
import os

filename = sys.argv[1]
directory = os.path.dirname(filename)

save  =False
if(len(sys.argv) == 3 and sys.argv[2] == "save"):
    save = True

data = np.genfromtxt(filename, skip_header=0)

t = len(data) * 0.02
stationary_end = 350
shaky_start    = 500
shaky_end      = shaky_start + stationary_end*3


acc_stationary = data[:stationary_end]
acc_shaky      = data[shaky_start:shaky_end]

stationary_mean = np.mean(acc_stationary)
shaky_mean      = np.mean(acc_shaky)

stationary_std  = np.std(acc_stationary)
shaky_std       = np.std(acc_shaky)


print("mean tationary", stationary_mean)
print("mean shaky", shaky_mean)
print("std stationary", stationary_std)
print("std shaky", shaky_std)


timestr = time.strftime("%Y%m%d_%H%M%S")

plt.plot(acc_stationary)
plt.plot(acc_shaky[::3])

plt.legend(["Stationary", "Rotating"])
plt.title("Figure 5: Acceleration time series")
plt.xlabel("t")
plt.ylabel("a (m/s^2)")


if(save):
    plt.savefig(os.path.join(directory, "acc_data_" + timestr + ".png"))


plt.show()



