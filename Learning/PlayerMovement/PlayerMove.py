import ctypes
from ctypes import *
import numpy as np 
from numpy.ctypeslib import ndpointer
#File to check that the rendezvous functions in C work in Python

import ctypes
import numpy as np

# Load the shared library
playermove = ctypes.CDLL('C:/Users/jt565/Desktop/PHD/rendezvoususingC/Learning/PlayerMovement/myb.dll')  # For Linux


#define function argument and return types
playermove.make_move.argtypes = (ctypes.c_int16,ctypes.c_int16,ctypes.c_int16, ctypes.c_char_p)
playermove.make_move.restype = ctypes.c_int

#Input variables
oldpos =0
coin =1
Ns =3 
graph_ = "cyclic"


graph = graph_.encode('utf-8')

#this works now but not well, for some reason - 
#it doesn't seem to move the players correctly it only goes to zero which implies it is a data translation error 


y = playermove.make_move(oldpos,coin,Ns,graph)
print(y)