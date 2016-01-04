
"""
https://github.com/Werkov/PyQt4/blob/master/examples/opengl/hellogl.py
https://github.com/prideout/coregl-python
http://stackoverflow.com/questions/11192938/compiling-shaders-in-pyqt

"""

import sys
import math

from PyQt4 import QtCore, QtGui, QtOpenGL
from PyQt4.QtGui import QWidget
from PyQt4.QtOpenGL import QGLShaderProgram, QGLShader, QGLFormat, QGLContext, QGLWidget

try:
    from OpenGL import GL
except ImportError:
    print "You need to install PyOpenGL"
    sys.exit(1)
else:
    print "OpenGL OK"
    
    
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
        #super(GLWidget, self).__init__(parent)
        
        if hasattr(QGLFormat, 'setVersion'):
            f = QGLFormat(); f.setVersion(3, 3)
            f.setProfile(QGLFormat.CoreProfile)
            c = QGLContext(f, None)
            QGLWidget.__init__(self, c, parent)
            print "Version is set to 3.3"
        else:
            QGLWidget.__init__(self, parent)
        
        self.backColor = QtGui.QColor.fromRgbF(1.0, 0.0, 0.0, 1.0)
        
    def minimumSizeHint(self):
        return QtCore.QSize(50, 50)
    
    def sizeHint(self):
        return QtCore.QSize(400, 400)
    
    def initializeGL(self):
        
        # shaders
        self._shaderProgram = QGLShaderProgram()
        if self._shaderProgram.addShaderFromSourceFile(QGLShader.Vertex, "shader.vert") :
            print "Vertex shader OK"
        if self._shaderProgram.addShaderFromSourceFile(QGLShader.Fragment, "shader.frag") :
            print "Fragment shader OK"
        print self._shaderProgram.log()
        self._shaderProgram.bind() # or link()
        
        
        self._mvpMatrixLocation = self._shaderProgram.uniformLocation("mvpMatrix")
        self._colorLocation = self._shaderProgram.attributeLocation("vertexColor")
        self._vertexLocation = self._shaderProgram.attributeLocation("vert")
        self._use_color_location = self._shaderProgram.uniformLocation("use_color")
                
        
        self.qglClearColor(self.backColor)
        GL.glEnable(GL.GL_BLEND);
        GL.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
        GL.glEnable(GL.GL_DEPTH_TEST)
        
        #print self.width()
        #print self.height()
    
    def paintGL(self):
        GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)
        GL.glLoadIdentity()
        
    #def updateGL(self):
    #    print "Update"
    
    
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