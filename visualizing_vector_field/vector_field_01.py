
# https://www.getdatajoy.com/examples/python-plots/vector-fields

from matplotlib.pyplot import cm
import matplotlib.pyplot as plt
import numpy as np

Y, X = np.mgrid[-10:10:20j, -10:10:20j]
U = -1 - np.cos(X**2 + Y)
V = 1 + X - Y
speed = np.sqrt(U**2 + V**2)
UN = U/speed
VN = V/speed

plot1 = plt.figure()
plt.quiver(X, Y, UN, VN,        # data
           U,                   # colour the arrows based on this array
           cmap=cm.seismic,     # colour map
           headlength=5)        # length of the arrows

plt.colorbar()                  # adds the colour bar

plt.title('Quive Plot, Dynamic Colours')
plt.show(plot1)                 # display the plot