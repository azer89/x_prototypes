# -*- coding: utf-8 -*-
"""
Created on Fri Feb 10 10:59:35 2017

@author: azer
"""

import numpy as np
import matplotlib.pylab as plt


def Barycentric(p, A, B, C):
    v0 = [B[0] - A[0], B[1] - A[1]]  # B - A
    v1 = [C[0] - A[0], C[1] - A[1]]  # C - A
    v2 = [p[0] - A[0], p[1] - A[1]]  # p -A
    d00 = np.dot(v0, v0)
    d01 = np.dot(v0, v1)
    d11 = np.dot(v1, v1)
    d20 = np.dot(v2, v0)
    d21 = np.dot(v2, v1)
    denom = d00 * d11 - d01 * d01
    v = (d11 * d20 - d01 * d21) / denom
    w = (d00 * d21 - d01 * d20) / denom
    u = 1.0 - v - w
    return u, v, w



A = [1.0, 2.0]
B = [2.0, 5.0]
C = [5.0, 3.0]

p = [3.0, 3.0]

u, v, w = Barycentric(p, A, B, C)
print u + v + w

p_new = [A[0] * u + B[0] * v + C[0] * w,  A[1] * u + B[1] * v + C[1] * w]

plt.figure(1)
plt.clf()
plt.plot(A[0], A[1],'o', color="blue", markersize=7)
plt.plot(B[0], B[1],'o', color="blue", markersize=7)
plt.plot(C[0], C[1],'o', color="blue", markersize=7)
plt.plot(p[0], p[1],'o', color="red", markersize=12)
plt.plot(p_new[0], p_new[1],'o', color="black", markersize=7)
plt.axis([0, 6, 0, 6])
plt.show()
