from ctypes import *
from numpy.ctypeslib import ndpointer
#File to check that the rendezvous functions in C work in Python
import numpy as np

#Load the library
wincheck = ctypes.CDLL('C:/Users/jt565/Desktop/PHD/rendezvoususingC/Learning/Win Checker/myb.dll') 

#Define input and output parameter types
wincheck.win_checker.argtypes = (ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int), ctypes.c_bool)
wincheck.win_checker.restype = ctypes.c_bool


#Call the funciton

old = np.array([1,0],dtype=np.int32)
new = np.array([0,1],dtype=np.int32)

if not old.flags['C_CONTIGUOUS']:
    old = np.ascontiguousarray(old, dtype=np.int32)

if not new.flags['C_CONTIGUOUS']:
    new = np.ascontiguousarray(new, dtype=np.int32)
# Pass the NumPy array to the C function
x = wincheck.win_checker(new.ctypes.data_as(ctypes.POINTER(ctypes.c_int)), old.ctypes.data_as(ctypes.POINTER(ctypes.c_int)),False)


x
#problem is with the edges == True bit in the code


