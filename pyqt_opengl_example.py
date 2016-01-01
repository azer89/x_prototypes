
"""
https://github.com/Werkov/PyQt4/blob/master/examples/opengl/hellogl.py
"""

import sys
import math

from PyQt4 import QtCore, QtGui, QtOpenGL

try:
    from OpenGL import GL
except ImportError:
    print "PyOpenGL must be installed to run this example"
    #app = QtGui.QApplication(sys.argv)
    #QtGui.QMessageBox.critical(None, "OpenGL hellogl", "PyOpenGL must be installed to run this example.")
    #sys.exit(1)