# Girl Animation
# 

from pyray import *
from tools import *


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

# Globals
anim_timer = 0.0
curr_anim = ANIM_ID_RUN
anim_idx = 0
camera = None

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

def animate_girl(canJump, face_right):
  global anim_timer
  global curr_anim
  global anim_idx
  delta = get_frame_time()
  anim_timer += delta

  #  if the time has expired select the next frame
  #  Se o timer espirou entao seleciona o proximo frame para ser exibido via incremento de anim_idx
  if anim_timer >= anim_array[curr_anim].frame_period:
    anim_timer = 0
    print("curr_anim = " + anim_array[curr_anim].name)
    if canJump == 0:
      # for jump we have a special sequence: 1,2,3 and loop 4 and 6 (indexes: 0,1,2 and loop 3 and 5):
      if anim_idx == 0: 
        anim_idx = 1
      elif anim_idx == 1:
        anim_idx = 2
      elif anim_idx == 2:
        anim_idx = 3
      elif anim_idx == 3:
        anim_idx = 5
      elif anim_idx == 5: 
        anim_idx = 3
      else:
        anim_idx = 0

    else:
      anim_idx = anim_idx + 1

    if anim_idx >= anim_array[curr_anim].num_frames: # se ultimo frame seleciona o primeiro
      anim_idx = 0
  
  if face_right == True:
    ret = anim_array[curr_anim].textures[anim_idx]
  else:
    ret = anim_array[curr_anim].flipped_textures[anim_idx]
    
  return ret

def UpdateCameraCenterMV():
  global camera
  camera.offset = Vector2(SCREEN_WIDTH/2.0, SCREEN_HEIGHT - 120)
  pos = Vector2(player.position.x, PLAYER_INITIAL_Y)
  camera.target = pos

def load_girl_textures():
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

def game():
  global camera
  currentFrame = 0
  framesCounter = 0
  framesSpeed = 8

  init_window(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [texture] example - sprite anim")

  #need to set in case of animation snyc
  set_target_fps(60)                 # Set our game to run at 60 frames-per-second

  load_girl_textures()

  camera = Camera2D()
  camera.target = player.position
  camera.offset = Vector2( SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0)
  camera.rotation = 0.0
  camera.zoom = 1.0

  UpdateCameraCenterMV()
  #position = Vector2(350.0, 280.0)

  # Main game loop
  while not window_should_close():  # Detect window close button or ESC key

      begin_drawing()

      clear_background(RAYWHITE)

      begin_mode_2d(camera)
      
      #draw sprite sheet texture
      girl_texture = animate_girl(False, True)

      menina_source = Rectangle(0,0, girl_texture.width, girl_texture.height)
      menina_dest = Rectangle( player.position.x + GIRL_POS_OFFSET_X, 
        player.position.y + GIRL_POS_OFFSET_Y, 
        girl_texture.width/4, girl_texture.height/4)

      menina_ori = Vector2(girl_texture.width/8, girl_texture.height/8)
      draw_texture_pro(girl_texture, menina_source,  menina_dest, menina_ori, 0, WHITE)

      end_mode_2d()

      end_drawing()

  # De-Initialization
  unload_texture(girl_texture)

  close_window()  # Close window and OpenGL context

if __name__ == "__main__":
  game()