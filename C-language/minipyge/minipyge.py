from pyray import *
#from tools import *
import time, sys

class NodeBase:
  all_nodes = []
  def __init__(self, name = None):
    self.queue = []
    self.name = name
    if name != None:
      self.all_nodes.append(self)
  
  def init(self):
    pass

  def run_slice(self, timestamp):
    pass

  def process_message(self, msg, timestamp):
    pass

  def run(self):
    for n in self.all_nodes:
      n.init()

    while True: # not window_should_close():
      for n in self.all_nodes:
        while len(n.queue) > 0:
          n.process_message(n.queue[0], get_frame_time())
          del(n.queue[0])

      for n in self.all_nodes:
        n.run_slice(get_frame_time())
      
      time.sleep(1)

  def send_message(self, node_dst, obj, node_from = None):
    node_dst.queue.append([obj, node_from])

class MiniGNode(NodeBase):
  def __init__(self, name):
    if name == None:
      print("MiniGNode: Name must be provided")
      sys.exit(1)
    super().__init__(name)

g_minipyge_root_node = NodeBase()

def minipyge_run():
  g_minipyge_root_node.run()
