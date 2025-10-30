
import sys

from common import *

#---- Import Mini Python Game Engine -----
MINI_PY_GE_DIR = "../minipyge"
sys.path.append(MINI_PY_GE_DIR)
from minipyge import *

STATIC_BACKGROUND_FILENAME = "assets/background-1280x960.png"
MOVING_BACKGROUND_FILENAME = "assets/bk-move-6400x960.png"
GROUND_FILENAME = "assets/ground-01.png"

class Scene(MiniGNode):
  def __init__(self, name):
    super().__init__(name)
    self.bg_texture = None
    self.moving_bg_texture = None
    self.ground_texture = None
    self.player_position = None

  def set_player_position(self, player_position):
    self.player_position = player_position
  
  def on_init(self):
    temp_image = load_image(STATIC_BACKGROUND_FILENAME)   # Loaded in CPU memory (RAM)
    self.bg_texture = load_texture_from_image(temp_image);          # Image converted to texture, GPU memory (VRAM)
    unload_image(temp_image);   # Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

    temp_image = load_image(MOVING_BACKGROUND_FILENAME)
    self.moving_bg_texture = load_texture_from_image(temp_image)
    unload_image(temp_image)

    temp_image = load_image(GROUND_FILENAME)
    self.ground_texture = load_texture_from_image(temp_image)
    unload_image(temp_image)

  def on_draw_2d(self, timestamp):
    moving_bg_texture_x = self.player_position.x / 2
    draw_texture(self.bg_texture, int(self.player_position.x - PLAYER_INITIAL_X - BACKGROUND_OFFSET), 0, WHITE)
    draw_texture(self.moving_bg_texture, int(moving_bg_texture_x - (BACKGROUND_OFFSET * 4)), 0, WHITE)
    draw_texture(self.ground_texture, 0, 0, WHITE)