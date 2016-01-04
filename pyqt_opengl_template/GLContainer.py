
"""
GLContainer
"""

from PyQt4 import QtGui
from PyQt4.QtGui import QWidget

from GLWidget import GLWidget

class GLContainer(QWidget):
    """
    Class GLContainer
    """
    def __init__(self):
        super(GLContainer, self).__init__()
        
        self.glWidget = GLWidget()
        
        mainLayout = QtGui.QHBoxLayout()
        
        mainLayout.addWidget(self.glWidget)
        self.setLayout(mainLayout)
        
        self.setWindowTitle("Hello GL")
        