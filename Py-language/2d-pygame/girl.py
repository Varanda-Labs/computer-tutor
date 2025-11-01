"""

                             MiniPyGE Demo 2D Game 

 By: Marcelo Varanda
 Copyrights 2025 Varanda Labs Inc. 

 raylib API: https://electronstudio.github.io/raylib-python-cffi/pyray.html

"""

import sys, time
from common import *

#---- Import Mini Python Game Engine -----
MINI_PY_GE_DIR = "../minipyge"
sys.path.append(MINI_PY_GE_DIR)
from minipyge import *

ANIM_ID_IDLE   =    0
ANIM_ID_RUN    =    1
ANIM_ID_JUMP   =    2
ANIM_ID_SHOOT  =    3
ANIM_ID_SLIDE  =    4
ANIM_ID_DEAD   =    5
ANIM_ID_MELEE  =    6

G = 400
PLAYER_JUMP_SPD = 420.0 
PLAYER_HOR_SPD = 200.0

GIRL_DIR = "./assets/girl/"
GIRL_POS_OFFSET_X = 8
GIRL_POS_OFFSET_Y = -60

class Girl(MiniGNode):
  def __init__(self, name, position):
    super().__init__(name)
    self.position = position  # Vector2 position
    self.speed = 0              # float speed
    self.canJump = False        # bool canJump
    self.state = 0              # int
    self.old_state = 0          #int
    self.game_node = None
    self.anim_timer = 0
    self.curr_anim = ANIM_ID_RUN
    self.anim_idx = 0
    self.face_right = 1
    self.floor_blocks = None

  def set_floor_blocks(self, blocks):
    self.floor_blocks = blocks

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

  def setGameNode(self, game_node):
    self.game_node = game_node

  def UpdateGirlState(self, state):
    if self.state != state:
      self.anim_idx = 0
      self.curr_anim = state
    self.old_state = self.state
    self.state = state

  def on_init(self):
    self.load_girl_textures()

  def on_slice(self, timestamp):
    delta = timestamp

    if is_key_down(KEY_LEFT) == True:
      self.position.x -= PLAYER_HOR_SPD * delta
      self.face_right = 0
      if self.canJump == True:
        self.UpdateGirlState(ANIM_ID_RUN)

    elif is_key_down(KEY_RIGHT) == True:
      self.position.x += PLAYER_HOR_SPD * delta
      self.face_right = 1
      if self.canJump == True:
        self.UpdateGirlState(ANIM_ID_RUN)

    else:
      if self.canJump == True:
        self.UpdateGirlState(ANIM_ID_IDLE)

    if (((is_key_down(KEY_SPACE) == True) and (self.canJump == True)) or
        ((is_key_down(KEY_UP) == True) and (self.canJump == True))):
      self.speed = -PLAYER_JUMP_SPD
#ifdef LIMIT_SINGLE_JUMP
      self.canJump = False
      self.UpdateGirlState(ANIM_ID_JUMP)
#endif
    hitObstacle = False
    in_air = True
    p = self.position
    for b in self.floor_blocks:
      if  b.x <= p.x and \
          b.x + b.width >= p.x and \
          b.y >= p.y and \
          b.y <= p.y + self.speed*delta:

        hitObstacle = True
        self.speed = 0.0
        p.y = b.y
        in_air = False
        self.curr_anim = self.state
        break

    if in_air == True: 
      # if (log) printf("in air %d\n", log_cnt);
      # if the girl is running we change the animation to jump as she is falling
      if self.state == ANIM_ID_RUN:
          self.curr_anim = ANIM_ID_JUMP

    if hitObstacle == False:
      self.position.y += self.speed*delta
      self.speed += G*delta
      self.canJump = False
    else:
      self.canJump = True

    self.send_message(self.game_node, self.position, self) 

  def animate_girl(self):
    delta = get_frame_time()
    self.anim_timer += delta

    #  if the time has expired select the next frame
    #  Se o timer espirou entao seleciona o proximo frame para ser exibido via incremento de self.anim_idx
    if self.anim_timer >= anim_array[self.curr_anim].frame_period:
      self.anim_timer = 0
      if self.canJump == False:
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
    
    if self.face_right == 1:
      ret = anim_array[self.curr_anim].textures[self.anim_idx]
    else:
      ret = anim_array[self.curr_anim].flipped_textures[self.anim_idx]
      
    return ret

  def on_draw_2d(self, timestamp):
    girl_texture = self.animate_girl()

    girl_source = Rectangle(0,0, girl_texture.width, girl_texture.height)
    girl_dest = Rectangle( self.position.x + GIRL_POS_OFFSET_X, 
      self.position.y + GIRL_POS_OFFSET_Y, 
      girl_texture.width/4, girl_texture.height/4)

    girl_ori = Vector2(girl_texture.width/8, girl_texture.height/8)
    draw_texture_pro(girl_texture, girl_source,  girl_dest, girl_ori, 0, WHITE)

class AnimInfo:
  def __init__(self, id, name, filename, num_frames, frame_period = 0.1):
    self.anim_id = id
    self.name = name
    self.filename_format = filename
    self.num_frames = num_frames
    self.textures = []
    self.flipped_textures = []
    self.frame_period = frame_period


anim_array = [
  #--------------- idle ----------------
  AnimInfo(ANIM_ID_IDLE, "Idle", GIRL_DIR + "Idle-XX.png", 10),

  #--------------- run ----------------
  AnimInfo(ANIM_ID_RUN, "Run", GIRL_DIR + "Run-XX.png", 8, 0.10),

  #--------------- jump ----------------
  AnimInfo(ANIM_ID_JUMP, "Jump", GIRL_DIR + "Jump-XX.png", 10),

  #--------------- shoot ----------------
  AnimInfo(ANIM_ID_SHOOT, "Shoot", GIRL_DIR +"Shoot-XX.png", 3),

  #--------------- slide ----------------
  AnimInfo(ANIM_ID_SLIDE, "Slide", GIRL_DIR + "Slide-XX.png", 5),

  #--------------- dead ----------------
  AnimInfo(ANIM_ID_DEAD, "Dead", GIRL_DIR + "Dead-XX.png", 10),

  #--------------- MeLee ----------------
  AnimInfo(ANIM_ID_MELEE, "MeLee", GIRL_DIR + "Melee-XX.png", 7),
]