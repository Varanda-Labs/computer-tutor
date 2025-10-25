# Tested with version: 5.5.0.2
# by @Lightnet

from pyray import *
from tools import *


# Initialization
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 450

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

anim_array = (
  #--------------- idle ----------------
  {
    "anim_id": ANIM_ID_IDLE,
    "name": "Idle",
    "filename_format": MENINA_DIR + "Idle-XX.png",
    "num_frames": 10,
    "frame_period": 0.1,
    "textures": [],
    "flipped_textures": []
  },
  #--------------- run ----------------
  {
    "anim_id": ANIM_ID_RUN,
    "name": "Run",
    "filename_format": MENINA_DIR + "Run-XX.png",
    "num_frames": 8,
    "frame_period": 0.1,
    "textures": [],
    "flipped_textures": []
  },
  #--------------- jump ----------------
  {
    "anim_id": ANIM_ID_JUMP,
    "name": "Jump",
    "filename_format": MENINA_DIR + "Jump-XX.png",
    "num_frames": 10,
    "frame_period": 0.1,
    "textures": [],
    "flipped_textures": []
  },
  #--------------- shoot ----------------
  {
    "anim_id": ANIM_ID_SHOOT,
    "name": "Shoot",
    "filename_format": MENINA_DIR + "Shoot-XX.png",
    "num_frames": 3,
    "frame_period": 0.1,
    "textures": [],
    "flipped_textures": []
  },
  #--------------- slide ----------------
  {
    "anim_id": ANIM_ID_SLIDE,
    "name": "Slide",
    "filename_format": MENINA_DIR + "Slide-XX.png",
    "num_frames": 5,
    "frame_period": 0.1,
    "textures": [],
    "flipped_textures": []
  },
  #--------------- dead ----------------
  {
    "anim_id": ANIM_ID_DEAD,
    "name": "Dead",
    "filename_format": MENINA_DIR + "Dead-XX.png",
    "num_frames": 10,
    "frame_period": 0.1,
    "textures": [],
    "flipped_textures": []
  },
  #--------------- MeLee ----------------
  {
    "anim_id": ANIM_ID_MELEE,
    "name": "MeLee",
    "filename_format": MENINA_DIR + "Melee-XX.png",
    "num_frames": 7,
    "frame_period": 0.1,
    "textures": [],
    "flipped_textures": []
  }
)

def load_girl_textures():
  for anim in anim_array:
    num_frames = anim["num_frames"]
    for i in range(num_frames):
      temp_text = anim["filename_format"].replace("XX", str(i+1))
      print("texture: " + temp_text)
      img= load_image(temp_text)
      txt = load_texture_from_image(img)
      image_flip_horizontal(img)
      txt_flip = load_texture_from_image(img)
      anim["textures"].append(txt)
      anim["flipped_textures"].append(txt_flip)

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
      draw_texture(scarfy, 15, 40, WHITE)
      #draw sprite animation
      draw_texture_rec(scarfy, frameRec, position,WHITE)

      draw_text("(c) Scarfy sprite by Eiden Marsal", SCREEN_WIDTH - 200, SCREEN_HEIGHT - 20, 10, GRAY)

      end_drawing()

  # De-Initialization
  unload_texture(scarfy)

  close_window()  # Close window and OpenGL context

if __name__ == "__main__":
  game()