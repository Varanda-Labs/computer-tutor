#

"""

                             MiniPyGE Demo 2D Game 

 By: Marcelo Varanda
 Copyrights 2025 Varanda Labs Inc. 

 raylib API: https://electronstudio.github.io/raylib-python-cffi/pyray.html

"""

import sys, time

#---- Import Mini Python Game Engine -----
MINI_PY_GE_DIR = "../minipyge"
sys.path.append(MINI_PY_GE_DIR)
from minipyge import *

from common import *
from girl import *
from scene import *
from ui_control import *

class GameNode(MiniGNode):
  def __init__(self, name):
    super().__init__(name)
    self.girl = Girl("girl", Vector2(PLAYER_INITIAL_X, PLAYER_INITIAL_Y))
    self.scene = Scene("scene")
    self.uicontrols = UIControls()
    self.scene.set_girl_position(self.girl.position)
    self.girl.setGameNode(self)
    self.girl_position = None # received via message

    # set z_pos: lower number last to be render (on top)
    self.uicontrols.z_pos = 10
    self.girl.z_pos = 20
    self.scene.z_pos = 30

    # girl needs to know where the floor blocks are:
    self.girl.set_floor_blocks(self.scene.get_floor_blocks())

  def on_message(self, msg, timestamp):
    #print(self.name + " got a message from " + msg[1].name)
    self.girl_position = msg[0]
    self.scene.set_girl_position(msg[0])

  def UpdateCameraCenterMV(self):
    self.camera.offset = Vector2(SCREEN_WIDTH/2.0, SCREEN_HEIGHT - 120)
    pos = Vector2(self.girl.position.x, PLAYER_INITIAL_Y)
    self.camera.target = pos

  def on_init(self):
    currentFrame = 0
    framesCounter = 0
    framesSpeed = 8

    init_window(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Python game")

    set_target_fps(60)

    self.camera = Camera2D()
    self.camera.target = self.girl.position
    self.camera.offset = Vector2( SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0)
    self.camera.rotation = 0.0
    self.camera.zoom = 1.0

    minipyge_set_2d(self.camera)

    self.UpdateCameraCenterMV()
  
  def on_slice(self, timestamp):
    self.UpdateCameraCenterMV()

  def on_destroy(self):
    close_window()

if __name__ == "__main__":
  game = GameNode("Main-node")
  minipyge_run()
