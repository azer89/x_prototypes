
"""
2D Autocorrelation
"""

import numpy as np
import matplotlib
import matplotlib.pylab as plt


# http://stackoverflow.com/questions/12201577/how-can-i-convert-an-rgb-image-into-grayscale-in-python
def rgb2gray(rgb):
    return np.dot(rgb[...,:3], [0.299, 0.587, 0.144])    
    
def GetPeaks(array_2d, tol = 2000):
    
    max_val = np.amax(array_2d)
    height, width = array_2d.shape

    peaks = []    
    
    for y_iter in xrange(height):
        for x_iter in xrange(width):
            val = array_2d[y_iter][x_iter]
            d = np.abs(val - max_val)
            #print d
            if(d < tol ):
                #print d
                peaks.append([x_iter, y_iter])
    
    return peaks
    


img_col = matplotlib.image.imread("jones.png")
img_col = (img_col * 255).astype(np.uint8)
img_gray = rgb2gray(img_col)

#width, height = np.shape(img_gray)

# go to frequency domain
F = np.fft.fft2(img_gray) 

F_conj_F = np.conj(-F) * F
length = np.linalg.norm(F_conj_F)

# bring back to spatial domain
f_ac = np.fft.ifft2(F_conj_F / length) 

f_ac_abs = np.abs(f_ac) * 1e10

peaks = GetPeaks(f_ac_abs)


plt.figure(1)
plt.clf()
plt.imshow(img_col)
for i in xrange(len(peaks) ):
    p = peaks[i]
    plt.plot(p[0],  p[1], 'x', color="red")
    #print p[0], " - ", p[1]


plt.figure(2)
plt.clf()
plt.imshow(img_gray, cmap='gray')


plt.figure(3)
plt.clf()
plt.imshow(np.absolute(f_ac), cmap='gray') 
 