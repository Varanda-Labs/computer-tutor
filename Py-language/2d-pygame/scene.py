"""

                          MiniPyGE Demo 2D Game 

 By: Marcelo Varanda
 Copyrights 2025 Varanda Labs Inc. 

 raylib API: https://electronstudio.github.io/raylib-python-cffi/pyray.html

"""

import sys

from common import *

#---- Import Mini Python Game Engine -----
MINI_PY_GE_DIR = "../minipyge"
sys.path.append(MINI_PY_GE_DIR)
from minipyge import *

"""
Info from GIMP file:
===================


box-1 pos: 293, 833   size: 1792x128
box-2 pos: 892, 627   size: 385x96   (plataforma)
box-3 pos: 1324, 497  size: 515x96    (plataforma)
box-4 pos: 2342, 829  size: 764x130
box-5 pos: 3108, 577  size: 643x383
box-6 pos: 4003, 431  size: 512x96     (plataforma)
box-7 pos: 4774, 831  size: 1627x130
box-8 pos: 2801, 671  size 257x96

bush-1 pos: 4130, 359  size: 133x73

Cactos-1 pos: 1442, 377  size: 106x119
Cactos-2 pos: 2626, 734  size: 92x98
Cactos-3 pos: 3256, 464  size: 109x117
Cactos-4 pos: 5026, 718  size: 111x112
Cactos-5 pos: 5593, 721  size: 112x110
"""

STATIC_BACKGROUND_FILENAME = "assets/background-1280x960.png"
MOVING_BACKGROUND_FILENAME = "assets/bk-move-6400x960.png"
GROUND_FILENAME = "assets/ground-01.png"


class Scene(MiniGNode):
  def __init__(self, name):
    super().__init__(name)
    self.bg_texture = None
    self.moving_bg_texture = None
    self.ground_texture = None
    self.girl_position = None
    self.floor_blocks = [
      Rectangle(293, 833, 1792, 128),
      Rectangle(892, 627, 385, 96 ),      #  (plataforma)
      Rectangle(1324, 497, 515, 96 ),     #  (plataforma)
      Rectangle(2342, 829,  764, 130 ),
      Rectangle(3108, 577,  643, 383),
      Rectangle(4003, 431,  512, 96  ),   #  (plataforma)
      Rectangle(4774, 831,  1627, 130),
      Rectangle(2801, 671, 257, 96)
    ]
  
  def get_floor_blocks(self):
    return self.floor_blocks

  def set_girl_position(self, girl_position):
    self.girl_position = girl_position
  
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
    moving_bg_texture_x = self.girl_position.x / 2
    draw_texture(self.bg_texture, int(self.girl_position.x - PLAYER_INITIAL_X - BACKGROUND_OFFSET), 0, WHITE)
    draw_texture(self.moving_bg_texture, int(moving_bg_texture_x - (BACKGROUND_OFFSET * 4)), 0, WHITE)
    draw_texture(self.ground_texture, 0, 0, WHITE)