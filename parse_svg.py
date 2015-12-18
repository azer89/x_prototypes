
"""
I'm still trying to find out the best way to parse and render svg files

Parse SVG
"""

import pysvg.parser
import cairo
import svglib

#from pysvg.structure import svg
from pysvg.builders import ShapeBuilder

svg_img = pysvg.parser.parse("ellipse.svg")
print "size :", svg_img.get_width(), svg_img.get_height()