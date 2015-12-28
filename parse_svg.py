
"""
I'm still trying to find out the best way to parse and render svg files

Parse SVG
"""

import pysvg.parser
import cairo
import svglib


from pysvg.builders import ShapeBuilder

from pysvg.structure import *
from pysvg.core import *
from pysvg.text import *

"""
# Example 1. load an svg then save it as another file
svg_img = pysvg.parser.parse("ellipse.svg")
svg_img.save("out_files/ellipse.svg")
"""

# Example 2. draw something
svg_img = Svg(0, 0, width = "100%", height = "100%")
txt = Text("HELLO WORLD", x = 0, y = 15)
txt.set_stroke_width("1px")
txt.set_stroke("#00C")
txt.set_fill("none")
txt.set_font_size("24")

group = G()
group.set_transform("rotate(-30)")
group.addElement(txt)

svg_img.addElement(group)
svg_img.save("out_files/hello_world.svg")

