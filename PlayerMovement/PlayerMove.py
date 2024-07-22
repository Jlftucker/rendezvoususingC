import ctypes
from ctypes import *
import numpy as np 
from numpy.ctypeslib import ndpointer
#File to check that the rendezvous functions in C work in Python

import ctypes
import numpy as np

# Load the shared library
playermove = ctypes.CDLL('C:/Users/jt565/Desktop/PHD/rendezvoususingC/PlayerMovement/mylib.dll', winmode=0)  # For Linux
# lib = ctypes.CDLL('./libexample.dll')  # For Windows
# lib = ctypes.CDLL('./libexample.dylib')  # For macOS


# Define the argument and return types
playermove.win_checker.argtypes = (ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int), ctypes.c_bool)
playermove.win_checker.restype = ctypes.c_int

old = np.array([0,1])
new = np.array([2,2])

# Ensure that the array is contiguous (C-style)
if not old.flags['C_CONTIGUOUS']:
    old = np.ascontiguousarray(old, dtype=np.int32)

if not new.flags['C_CONTIGUOUS']:
    new = np.ascontiguousarray(new, dtype=np.int32)
# Pass the NumPy array to the C function
x = playermove.win_checker(new.ctypes.data_as(ctypes.POINTER(ctypes.c_int)), old.ctypes.data_as(ctypes.POINTER(ctypes.c_int)),False)


## IT returns 0 for false and 1 for True
print(x)



#Now lets try to import the player move function

#define function argument and return types
playermove.make_move.argtype(ctypes.c_int16(),ctypes.c_int16(),ctypes.c_int16(),ctypes.c_wchar_p)
playermove.make_move.restype = ctypes.c_int()

#Input variables
oldpos = 1
coin = 0
Ns =3 
graph = "cyclic"

y = playermove.make_move(oldpos,coin,Ns,graph)