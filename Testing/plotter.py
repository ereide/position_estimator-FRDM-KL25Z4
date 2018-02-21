#plotter.py
#Copyright Eivind Roson Eide 2018

#use: python plotter.pu out/file.txt [save]

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys
import time
import os

filename = sys.argv[1]
directory = os.path.dirname(filename)

save  =False
if(len(sys.argv) == 3 and sys.argv[2] == "save"):
    save = True

data = np.genfromtxt(filename, delimiter=',', skip_header=1)

t       = data[:, 0]*0.1
z_h     = data[:, 1]
z_a     = data[:, 2]
h       = data[:, 3]
v       = data[:, 4]
h_est   = data[:, 5]
v_est   = data[:, 6]
a_est   = data[:, 7]
h_var   = data[:, 8]
v_var   = data[:, 9]



timestr = time.strftime("%Y%m%d_%H%M%S")
fig_file_name = os.path.splitext(os.path.basename(filename))[0]

fig, ax1 = plt.subplots()
ax2 = ax1.twinx()

ax1.plot(t, z_h, 'ro', t, h, 'b-', t, h_est, 'g-')
ax2.plot(t, h_var, "y-")
ax1.legend(["h_measured", "h_actual", "h_estimate"])
ax2.legend(["log(h_var)"])
plt.xlabel("t")
plt.ylabel("h (m)")
#ax2.ylabel("log(Var(h))")

if(save):
    plt.savefig(os.path.join(directory, "pos_" + fig_file_name + "_" + timestr + ".png"))

fig, ax1 = plt.subplots()
ax2 = ax1.twinx()

ax1.plot(t, v,   'b-', t, v_est, 'g-')
ax2.plot(t, v_var, "y-")

ax1.legend(["v_actual", "v_estimate"])
ax2.legend(["log(v_var)"])

plt.xlabel("t")
plt.ylabel("v (m/s)")
#ax2.ylabel("log(Var(v))")

if(save):
    plt.savefig(os.path.join(directory, "vel_" + fig_file_name + "_" + timestr + ".png"))

plt.show()
