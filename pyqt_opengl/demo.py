import ctypes

import numpy
from OpenGL.GL import *

from PyQt4 import QtGui, QtOpenGL
from PyQt4.QtOpenGL import QGLShaderProgram, QGLShader
from PyQt4.QtGui import QMatrix4x4

VERTEX_SHADER = """#version 330

//layout (location=0) in vec4 position;
//layout (location=1) in vec4 color;
//layout (location=2) in vec2 uv;
in vec4 position;
in vec4 color;
in vec2 uv;


out vec2 theUV;
out vec4 theColor;

uniform mat4 mvpMatrix;

void main()
{
    gl_Position = mvpMatrix * position;
    theColor = color;
    theUV = uv;
}
"""

FRAGMENT_SHADER = """#version 330
in vec2 theUV;
in vec4 theColor;
out vec4 outputColor;

//uniform float lolol;
uniform float use_color;

void main()
{
    outputColor = theColor;
    //outputColor = vec4(lolol, 0.0, 0.0, 1.0);
}
"""

class MyWidget(QtOpenGL.QGLWidget):
    def initializeGL(self):
        glViewport(0, 0, self.width(), self.height())

        self._shaderProgram = QGLShaderProgram()
        if self._shaderProgram.addShaderFromSourceCode(QGLShader.Vertex, VERTEX_SHADER) :
            print "Vertex shader OK"
        if self._shaderProgram.addShaderFromSourceCode(QGLShader.Fragment, FRAGMENT_SHADER) :
            print "Fragment shader OK"
        print self._shaderProgram.log()
        self._shaderProgram.link()

        self._mvpMatrixLocation  = self._shaderProgram.uniformLocation("mvpMatrix")
        self._colorLocation      = self._shaderProgram.attributeLocation("color");
        self._vertexLocation     = self._shaderProgram.attributeLocation("position");
        self._use_color_location = self._shaderProgram.uniformLocation("use_color");
        self._texCoordLocation   = self._shaderProgram.attributeLocation("uv");

        """
        vertexData = numpy.array([
                                0.0, 50, 0.0, 1.0,
                                50, -36.6, 0.0, 1.0,
                                -50, -36.6, 0.0, 1.0,

                                0.0,   35, 0.0, 1.0,
                                35, -23.66, 0.0, 1.0,
                                -35, -23.66, 0.0, 1.0,

                                -20, 15, 0.0, 1.0,
                                15, -43.66, 0.0, 1.0,
                                -55, -43.66, 0.0, 1.0,

                                1.0, 1.0, 1.0, 1.0,
                                1.0, 1.0, 0.0, 1.0,
                                1.0, 0.0, 1.0, 1.0,

                                1.0, 0.0, 0.0, 1.0,
                                1.0, 0.0, 0.0, 1.0,
                                1.0, 0.0, 0.0, 1.0,

                                0.0, 1.0, 0.0, 1.0,
                                0.0, 1.0, 0.0, 1.0,
                                0.0, 1.0, 0.0, 1.0,

                                ],

                                dtype=numpy.float32)
        """

        vertexData = numpy.array([
                                20.0, 20,   0.0, 1.0,
                                500,  10.0, 0.0, 1.0,
                                10,   100,  0.0, 1.0,

                                0.0, 1.0, 0.0, 1.0,
                                0.0, 1.0, 0.0, 1.0,
                                0.0, 1.0, 0.0, 1.0,],

                                dtype=numpy.float32)


        # create VAO
        self.VAO = glGenVertexArrays(1)
        glBindVertexArray(self.VAO)

        # create VBO
        VBO = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, VBO)
        glBufferData(GL_ARRAY_BUFFER, vertexData.nbytes, vertexData, GL_STATIC_DRAW)

        """
        glEnableVertexAttribArray(0)
        glEnableVertexAttribArray(1)
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, None)
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, ctypes.c_void_p(48))
        """

        glEnableVertexAttribArray(self._vertexLocation)
        glEnableVertexAttribArray(self._colorLocation)
        glVertexAttribPointer(self._vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, None)
        glVertexAttribPointer(self._colorLocation, 4, GL_FLOAT, GL_FALSE, 0, ctypes.c_void_p(48))

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindVertexArray(0)

    def paintGL(self):
        glClearColor(0, 0, 0, 1)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        orthoMatrix = QMatrix4x4()
        orthoMatrix.ortho(0.0, self.width() , self.height(), 0, -100, 100)
        transformMatrix = QMatrix4x4()
        transformMatrix.setToIdentity()
        mpvMatrix = orthoMatrix * transformMatrix

        # activate shader program
        self._shaderProgram.bind()
        #loc = self._shaderProgram.uniformLocation("lolol")
        #self._shaderProgram.setUniformValue(loc, 1.0)
        self._shaderProgram.setUniformValue(self._mvpMatrixLocation, mpvMatrix)

        glBindVertexArray(self.VAO)

        # draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 9)

        glBindVertexArray(0)
        glUseProgram(0)


def main():
    import sys

    app = QtGui.QApplication(sys.argv)

    glformat = QtOpenGL.QGLFormat()
    glformat.setVersion(3, 3)
    glformat.setProfile(QtOpenGL.QGLFormat.CoreProfile)
    w = MyWidget(glformat)
    w.resize(640, 480)
    w.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()