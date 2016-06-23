

from __future__ import division
import numpy as np
import matplotlib.pylab as plt
plt.ion()

x = np.arange(0, 1, 0.01)

# outside function f(x)
alpha = 0.6 * np.sqrt(2)
f = 1 - alpha * x

plt.figure(1)
plt.clf()
plt.title("outside function f(x)")
plt.plot(x, f, color='blue')
plt.show()

# inside function g(x)
phi = np.max(x)
t = -1.0 * (x * x) / (2.0 * (0.01 * phi) * (0.01 * phi))
g = np.exp(t)

plt.figure(2)
plt.clf()
plt.title("inside function g(x)")
plt.plot(x, g, color='blue')
plt.show()
