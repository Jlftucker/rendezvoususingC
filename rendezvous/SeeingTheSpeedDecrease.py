import ctypes
from ctypes import *
import numpy as np 
from numpy.ctypeslib import ndpointer
#File to check that the rendezvous functions in C work in Python
import time as t
import ctypes
import numpy as np

game = ctypes.CDLL('C:/Users/jt565/Desktop/PHD/rendezvoususingC/Learning/PlayAGame/mylib.dll')  # For Linux


game.main.argtype = (c_int16, c_int,c_int,c_bool)
game.main.restype = c_float

start= t.time()
y = game.main(3,100000,1,True)
end = t.time()

print(end-start)

print(y)