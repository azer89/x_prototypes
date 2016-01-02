
"""
https://github.com/Werkov/PyQt4/blob/master/examples/opengl/hellogl.py
"""

import sys
import math

from PyQt4 import QtCore, QtGui, QtOpenGL
from PyQt4.QtGui import QWidget

try:
    print "Do we have OpenGL ?"
    from OpenGL import GL
except ImportError:
    print "NO, you need to install PyOpenGL dude"
    sys.exit(1)
else:
    print "YES !"
    
class Window(QtGui.QWidget):
    """
    Class Window
    """
    def __init__(self):
        super(Window, self).__init__()
        
        self.glWidget = GLWidget()
        
        mainLayout = QtGui.QHBoxLayout()
        mainLayout.addWidget(self.glWidget)
        self.setLayout(mainLayout)
        
        self.setWindowTitle("Hello GL")
    
class GLWidget(QtOpenGL.QGLWidget):
    """
    Class GLWidget
    """
    
    def __init__(self, parent=None):
        super(GLWidget, self).__init__(parent)
        
        self.backColor = QtGui.QColor.fromRgbF(1.0, 0.0, 0.0, 1.0)
        
    def minimumSizeHint(self):
        return QtCore.QSize(50, 50)
    
    def sizeHint(self):
        return QtCore.QSize(400, 400)
    
    def initializeGL(self):
        self.qglClearColor(self.backColor)
        #self.object = self.makeObject()
        GL.glShadeModel(GL.GL_FLAT)
        GL.glEnable(GL.GL_DEPTH_TEST)
        GL.glEnable(GL.GL_CULL_FACE)
    
    def paintGL(self):
        GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)
        GL.glLoadIdentity()
    
    
if __name__ == '__main__':
    app_created = False

    app = QtCore.QCoreApplication.instance()
    
    if app is None:
        app = QtGui.QApplication(sys.argv)
        app_created = True
    
    app.references = set()
    
    #window = QWidget()
    window = Window()    
    window.resize(320, 240)
    #window.setWindowTitle("Hello World!")
    app.references.add(window)
    window.show()

    
    if app_created:
        app.exec_()