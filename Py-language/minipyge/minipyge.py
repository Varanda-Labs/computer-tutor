"""
  MiniPyGE - Mini Python Game Engine

  Tiny set of classes and methods to make raylib based games organized.
  The classes of the game can be derived from MiniGNode. Callbacks methods are
  automatically called to perform basic operations like draw.

  License: GPL3
  Copyright 2025, Varanda Labs Inc.
"""

from pyray import *
import time, sys

class privateNodeBase:
  """
    private class. Game should not use this class
  """
  # class variables (static)
  all_nodes = []
  sorted_nodes = None

  def __init__(self, name = None):
    # instance variables
    self.queue = []
    self.name = name
    self.mode3d = False
    self.camera = None
    self.z_pos = 0
    if name != None:
      privateNodeBase.all_nodes.append(self)
  
  def on_init(self):
    pass

  def on_slice(self, timestamp):
    pass

  def on_draw_canvas(self, timestamp):
    pass

  def on_draw_2d(self, timestamp):
    pass

  def on_draw_3d(self, timestamp):
    pass

  def on_message(self, msg, timestamp):
    pass

  def on_destroy(self):
    pass

  def send_message(self, node_dst, obj, node_from = None):
    node_dst.queue.append([obj, node_from])


    # static member
  def privateSortNodes():
    # sort all_nodes based on z_pos
    s = []
    for n in privateNodeBase.all_nodes:
      s.append([n.z_pos, n])
    ss = sorted(s, key=lambda x: x[0], reverse=True)
    privateNodeBase.sorted_nodes = []
    for n in ss:
      privateNodeBase.sorted_nodes.append(n[1])

  def privateRun(self):

    for n in privateNodeBase.all_nodes:
      n.on_init()

    privateNodeBase.privateSortNodes() # static member

    while not window_should_close():
      #------ Dispatch messages ------ 
      for n in privateNodeBase.sorted_nodes:
        while len(n.queue) > 0:
          n.on_message(n.queue[0], get_frame_time())
          del(n.queue[0])

      #------ on_slice's -------
      for n in privateNodeBase.sorted_nodes:
        n.on_slice(get_frame_time())

      #------ start drawing -------
      begin_drawing()

      #------ on_draw_2d's or on_draw_3d -------
      if self.mode3d == False:
        begin_mode_2d(self.camera)
        for n in privateNodeBase.sorted_nodes:
          n.on_draw_2d(get_frame_time())
        end_mode_2d()
      else:
        begin_mode_3d(self.camera)
        for n in privateNodeBase.sorted_nodes:
          n.on_draw_3d(get_frame_time())
        end_mode_2d()

      #------ on_draw_canvas's -------
      for n in privateNodeBase.sorted_nodes:
        n.on_draw_canvas(get_frame_time())

      end_drawing()

    #----- destruction 
    for n in privateNodeBase.all_nodes:
      n.on_destroy()



class MiniGNode(privateNodeBase):
  """
    MiniGNode is the class which all game classes should be derived from.
  """
  def __init__(self, name):
    if name == None:
      print("MiniGNode: Name must be provided")
      sys.exit(1)
    super().__init__(name)

  def on_init(self):
    """
      on_init: is called only once during initialization.
      The class should use this method to use raylib function like loading images, textures, models and etc.
      Note: The object constructor, the __init___ method, should not be used to call raylib functions. 
    """
    pass

  def on_slice(self, timestamp):
    """
      on_slice is called from the core loop right before on_draw_* being called.
      This is the place to do all logic operations prior to render each frame.
    """
    pass

  def on_draw_canvas(self, timestamp):
    """
      on_draw_canvas: proper method to draw 2D elements to the screen. 
      Usually UI screen overlays where camera position has ne affect.
    """
    pass

  def on_draw_2d(self, timestamp):
    """
      on_draw_2d: the core calls raylib's begin_mode_2d(self.camera) method prior calling this callback
      Note: the game code should have called minipyge_set_2d during initialization to provide the camera object.
    """
    pass

  def on_draw_3d(self, timestamp):
    """
      on_draw_3d: the core calls raylib's begin_mode_3d(self.camera) method prior calling this callback
      Note: the game code should have called minipyge_set_3d during initialization to provide the camera object.
    """
    pass

  def on_message(self, msg, timestamp):
    """
      on_message: any derived class can call send_message method passing as parameters:
      destination node and object.
      The on_message is called to deliver the object. msg is a list with two elements:
      the first is the sent object and the second element is the sender.
    """
    pass

##----------------- Functions -------------------

g_minipyge_root_node = privateNodeBase()

def minipyge_run():
  g_minipyge_root_node.privateRun()

def minipyge_set_2d(cam):
  g_minipyge_root_node.camera = cam
  g_minipyge_root_node.mode3d = False

def minipyge_set_3d(cam):
  g_minipyge_root_node.camera = cam
  g_minipyge_root_node.mode3d = True

