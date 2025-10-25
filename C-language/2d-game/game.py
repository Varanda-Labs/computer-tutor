# Tested with version: 5.5.0.2
# by @Lightnet

from pyray import *
from tools import *


# Initialization
# SCREEN_WIDTH = 800
# SCREEN_HEIGHT = 800 #450
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

anim_timer = 0.0
curr_anim = ANIM_ID_RUN
anim_idx = 0

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
  currentFrame = 0
  framesCounter = 0
  framesSpeed = 8

  init_window(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [texture] example - sprite anim")

  #need to set in case of animation snyc
  set_target_fps(60)                 # Set our game to run at 60 frames-per-second

  load_girl_textures()

  scarfy = load_texture("/home/mvaranda/lixo/pyraylib-tests/raylib-python-cffi/examples/textures/resources/scarfy.png")  # Texture loading

  frameRec = Rectangle(0.0, 0.0, scarfy.width/6, scarfy.height)

  position = Vector2(350.0, 280.0)

  # Main game loop
  while not window_should_close():  # Detect window close button or ESC key

      framesCounter += 1

      if framesCounter >= 60/framesSpeed:
          framesCounter = 0
          currentFrame += 1
          if currentFrame > 5:
              currentFrame = 0

          frameRec.x = float(currentFrame) * float(scarfy.width/6)
      # Control speed animation
      if (is_key_pressed(KeyboardKey.KEY_RIGHT)):
          framesSpeed += 1
      elif is_key_pressed(KeyboardKey.KEY_LEFT):
          framesSpeed -= 1

      if framesSpeed > MAX_FRAME_SPEED:
          framesSpeed = MAX_FRAME_SPEED
      elif framesSpeed < MIN_FRAME_SPEED:
          framesSpeed = MIN_FRAME_SPEED

      begin_drawing()

      clear_background(RAYWHITE)
      #draw sheet block
      draw_rectangle_lines(15, 40, scarfy.width, scarfy.height, LIME)
      #draw current frame render
      draw_rectangle_lines(15 + int(frameRec.x), 40 + int(frameRec.y), int(frameRec.width), int(frameRec.height), RED)
      draw_text("FRAME SPEED: ", 165, 210, 10, DARKGRAY)
      draw_text(f" FPS {framesSpeed}", 575, 210, 10, DARKGRAY) #format string
      draw_text("PRESS RIGHT/LEFT KEYS to CHANGE SPEED!", 290, 240, 10, DARKGRAY)
      #display bar framesSpeed cap
      for i in range(MAX_FRAME_SPEED):
          if i < framesSpeed:
              draw_rectangle(250 + 21*i, 205, 20, 20, RED)
          draw_rectangle_lines(250 + 21*i, 205, 20, 20, MAROON)
      #draw sprite sheet texture
      txt = animate_girl(False, True)
      #draw_texture(txt, 15, 40, WHITE)

      menina_source = Rectangle(0,0, txt.width, txt.height)
      #menina_dest = Rectangle(player.position.x + GIRL_POS_OFFSET_X, player.position.y + GIRL_POS_OFFSET_Y, menina_texture_sample_ptr->width/4, menina_texture_sample_ptr->height/4};
      menina_dest = Rectangle(150, 200, txt.width/4, txt.height/4)

      menina_ori = Vector2(txt.width/8, txt.height/8)
      draw_texture_pro(txt, menina_source,  menina_dest, menina_ori, 0, WHITE)


      #draw_texture(scarfy, 15, 40, WHITE)
      #draw sprite animation
      #draw_texture_rec(scarfy, frameRec, position,WHITE)

      draw_text("(c) Scarfy sprite by Eiden Marsal", SCREEN_WIDTH - 200, SCREEN_HEIGHT - 20, 10, GRAY)

      end_drawing()

  # De-Initialization
  unload_texture(scarfy)

  close_window()  # Close window and OpenGL context

if __name__ == "__main__":
  game()