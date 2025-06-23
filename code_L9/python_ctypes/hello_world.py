import ctypes
import os

clib = ctypes.CDLL(os.getcwd() + "/testlib.so")
hello = clib.hello_world
hello()

