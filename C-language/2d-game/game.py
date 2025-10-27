# Girl Animation
# 

import sys, time

#---- Import Mini Python Game Engine -----
MINI_PY_GE_DIR = "../minipyge"
sys.path.append(MINI_PY_GE_DIR)
from minipyge import *

# Initialization
SCREEN_WIDTH = int ((1280 / 4) * 3)
SCREEN_HEIGHT = int ((960 / 4) * 3)

PLAYER_INITIAL_X = 400
PLAYER_INITIAL_Y = 830

MAX_FRAME_SPEED = 15
MIN_FRAME_SPEED = 1

ANIM_ID_IDLE   =    0
ANIM_ID_RUN    =    1
ANIM_ID_JUMP   =    2
ANIM_ID_SHOOT  =    3
ANIM_ID_SLIDE  =    4
ANIM_ID_DEAD   =    5
ANIM_ID_MELEE  =    6

MENINA_DIR = "./assets/girl/"
GIRL_POS_OFFSET_X = 8
GIRL_POS_OFFSET_Y = -60


class Node(MiniGNode):
  def __init__(self, name):
    super().__init__(name)
  def on_init(self):
    print("init: " + self.name)
  def on_slice(self, timestamp):
    print("on_slice: " + self.name)
  def on_message(self, msg, timestamp):
    print("on_message " + self.name)
    print("message: " + msg[0])

def test():
  node_a = Node("node_a")
  node_b = Node("node_b")
  node_b.send_message(node_a, "This was a message sent from node B")
  minipyge_run()


class Player:
  def __init__(self, position):
    self.position = position  # Vector2 position
  position = None
  speed = 0              # float speed
  canJump = False        # bool canJump
  state = 0              # int
  old_state = 0          #int
  
  envItems = [
    Rectangle(293, 833, 1792, 128),
    Rectangle(892, 627, 385, 96),
    Rectangle(1324, 497, 515, 96),
    Rectangle(2342, 829,  764, 130),
    Rectangle(3108, 577,  643, 383),
    Rectangle(4003, 431,  512, 96 ),
    Rectangle(4774, 831,  1627, 130),
    Rectangle(2801, 671, 257, 96)
  ]

player = Player(Vector2(PLAYER_INITIAL_X, PLAYER_INITIAL_Y))

class AnimInfo:
  def __init__(self, id, name, filename, num_frames, frame_period = 0.1):
    self.anim_id = id
    self.name = name
    self.filename_format = filename
    self.num_frames = num_frames
    self.textures = []
    self.flipped_textures = []
    self.frame_period = frame_period


anim_array = (
  #--------------- idle ----------------
  AnimInfo(ANIM_ID_IDLE, "Idle", MENINA_DIR + "Idle-XX.png", 10),

  #--------------- run ----------------
  AnimInfo(ANIM_ID_RUN, "Run", MENINA_DIR + "Run-XX.png", 8, 0.15),

  #--------------- jump ----------------
  AnimInfo(ANIM_ID_JUMP, "Jump", MENINA_DIR + "Jump-XX.png", 10),

  #--------------- shoot ----------------
  AnimInfo(ANIM_ID_SHOOT, "Shoot", MENINA_DIR +"Shoot-XX.png", 3),

  #--------------- slide ----------------
  AnimInfo(ANIM_ID_SLIDE, "Slide", MENINA_DIR + "Slide-XX.png", 5),

  #--------------- dead ----------------
  AnimInfo(ANIM_ID_DEAD, "Dead", MENINA_DIR + "Dead-XX.png", 10),

  #--------------- MeLee ----------------
  AnimInfo(ANIM_ID_MELEE, "MeLee", MENINA_DIR + "Melee-XX.png", 7),
)

class GameNode(MiniGNode):
  def __init__(self, name):
    super().__init__(name)
    self.anim_timer = 0
    self.curr_anim = ANIM_ID_RUN
    self.anim_idx = 0

  def on_message(self, msg, timestamp):
    print("on_message " + self.name)
    print("message: " + msg[0])

  def animate_girl(self, canJump, face_right):
    delta = get_frame_time()
    self.anim_timer += delta

    #  if the time has expired select the next frame
    #  Se o timer espirou entao seleciona o proximo frame para ser exibido via incremento de self.anim_idx
    if self.anim_timer >= anim_array[self.curr_anim].frame_period:
      self.anim_timer = 0
      if canJump == 0:
        # for jump we have a special sequence: 1,2,3 and loop 4 and 6 (indexes: 0,1,2 and loop 3 and 5):
        if self.anim_idx == 0: 
          self.anim_idx = 1
        elif self.anim_idx == 1:
          self.anim_idx = 2
        elif self.anim_idx == 2:
          self.anim_idx = 3
        elif self.anim_idx == 3:
          self.anim_idx = 5
        elif self.anim_idx == 5: 
          self.anim_idx = 3
        else:
          self.anim_idx = 0

      else:
        self.anim_idx = self.anim_idx + 1

      if self.anim_idx >= anim_array[self.curr_anim].num_frames: # se ultimo frame seleciona o primeiro
        self.anim_idx = 0
    
    if face_right == True:
      ret = anim_array[self.curr_anim].textures[self.anim_idx]
    else:
      ret = anim_array[self.curr_anim].flipped_textures[self.anim_idx]
      
    return ret

  def UpdateCameraCenterMV(self):
    self.camera.offset = Vector2(SCREEN_WIDTH/2.0, SCREEN_HEIGHT - 120)
    pos = Vector2(player.position.x, PLAYER_INITIAL_Y)
    self.camera.target = pos

  def load_girl_textures(self):
    for anim in anim_array:
      num_frames = anim.num_frames
      for i in range(num_frames):
        temp_text = anim.filename_format.replace("XX", str(i+1))
        print("texture: " + temp_text)
        img= load_image(temp_text)
        txt = load_texture_from_image(img)
        image_flip_horizontal(img)
        txt_flip = load_texture_from_image(img)
        anim.textures.append(txt)
        anim.flipped_textures.append(txt_flip)

  def on_init(self):
    currentFrame = 0
    framesCounter = 0
    framesSpeed = 8

    init_window(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [texture] example - sprite anim")

    #need to set in case of animation snyc
    set_target_fps(60)                 # Set our game to run at 60 frames-per-second

    self.load_girl_textures()

    self.camera = Camera2D()
    self.camera.target = player.position
    self.camera.offset = Vector2( SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0)
    self.camera.rotation = 0.0
    self.camera.zoom = 1.0

    minipyge_set_2d(self.camera)

    self.UpdateCameraCenterMV()

  def on_draw_canvas(self, timestamp):
    clear_background(RAYWHITE)
    draw_text("Controls:", 20, 20, 10, BLACK)

  def on_draw_2d(self, timestamp):
    girl_texture = self.animate_girl(False, True)

    menina_source = Rectangle(0,0, girl_texture.width, girl_texture.height)
    menina_dest = Rectangle( player.position.x + GIRL_POS_OFFSET_X, 
      player.position.y + GIRL_POS_OFFSET_Y, 
      girl_texture.width/4, girl_texture.height/4)

    menina_ori = Vector2(girl_texture.width/8, girl_texture.height/8)
    draw_texture_pro(girl_texture, menina_source,  menina_dest, menina_ori, 0, WHITE)

  def on_destroy(self):
    close_window()

if __name__ == "__main__":
  game = GameNode("Main-node")
  minipyge_run()
