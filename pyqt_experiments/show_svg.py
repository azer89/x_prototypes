
import sys
from PyQt4.QtGui import *
from PyQt4 import QtCore, QtGui
from PyQt4.QtSvg import QSvgWidget

from PyQt4.QtGui import QApplication, QGraphicsScene, QGraphicsView
from PyQt4.QtWebKit import QGraphicsWebView

app_created = False

app = QtCore.QCoreApplication.instance()

if app is None:
    app = QtGui.QApplication(sys.argv)
    app_created = True

app.references = set()

widget = QSvgWidget("circle_star.svg")
widget.resize(500, 500)
widget.setWindowTitle("Hello SVG!")

app.references.add(widget)
widget.show()

if app_created:
    app.exec_()