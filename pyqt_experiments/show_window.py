
# http://cyrille.rossant.net/making-pyqt4-pyside-and-ipython-work-together/

import sys
from PyQt4.QtGui import *
from PyQt4 import QtCore, QtGui


app_created = False

app = QtCore.QCoreApplication.instance()

if app is None:
    app = QtGui.QApplication(sys.argv)
    app_created = True

app.references = set()

window = QWidget()
window.resize(320, 240)
window.setWindowTitle("Hello World!")
app.references.add(window)
window.show()

if app_created:
    app.exec_()