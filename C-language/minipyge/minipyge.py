from pyray import *
import time, sys

class privateNodeBase:
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

  def privateSortNodes(self):
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

    self.privateSortNodes()

    while not window_should_close():
      #------ Dispatch messages ------ 
      for n in privateNodeBase.sorted_nodes:
        while len(n.queue) > 0:
          n.on_message(n.queue[0], get_frame_time())
          del(n.queue[0])

      #------ on_slice's -------
      for n in privateNodeBase.sorted_nodes:
        n.on_slice(get_frame_time())

      #------ on_draw_canvas's -------
      begin_drawing()
      for n in privateNodeBase.sorted_nodes:
        n.on_draw_canvas(get_frame_time())

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
      end_drawing()

    #----- destruction 
    for n in privateNodeBase.all_nodes:
      n.on_destroy()
      
  def send_message(self, node_dst, obj, node_from = None):
    node_dst.queue.append([obj, node_from])



class MiniGNode(privateNodeBase):
  def __init__(self, name):
    if name == None:
      print("MiniGNode: Name must be provided")
      sys.exit(1)
    super().__init__(name)

g_minipyge_root_node = privateNodeBase()

def minipyge_run():
  g_minipyge_root_node.privateRun()

def minipyge_set_2d(cam):
  g_minipyge_root_node.camera = cam
  g_minipyge_root_node.mode3d = False

def minipyge_set_3d(cam):
  g_minipyge_root_node.camera = cam
  g_minipyge_root_node.mode3d = True

