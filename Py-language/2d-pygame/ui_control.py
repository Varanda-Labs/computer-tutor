import sys

from common import *

#---- Import Mini Python Game Engine -----
MINI_PY_GE_DIR = "../minipyge"
sys.path.append(MINI_PY_GE_DIR)
from minipyge import *

class UIControls(MiniGNode):
  def __init__(self):
    super().__init__("UIControls")

  def on_draw_canvas(self, timestamp):
    draw_text("Controls:", 20, 20, 10, BLACK)

