import os, sys, importlib

def check_path():
  found = False
  current_dir = os.getcwd()
  for s in sys.path:
    if s == current_dir:
      return
  sys.path.append(current_dir)

def reload(m):
  check_path()
  importlib.reload(m)