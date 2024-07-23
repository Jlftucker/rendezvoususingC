import ctypes
import numpy as np
import numpy as np 
from numpy.ctypeslib import ndpointer

#import dll file
index = ctypes.WinDLL('C:/Users/jt565/Desktop/PHD/rendezvoususingC/Learning/QuantumTableIndex/mylib.dll')


index.quantum_table_index.argtypes = (ctypes.c_int16, ctypes.c_int16, ctypes.c_int16, ctypes.c_char_p, ctypes.c_bool)

index.quantum_table_index.restype = ctypes.c_int


Ns = ctypes.c_int16(3)
playerpos1 =  ctypes.c_int16(2)
playerpos2 =  ctypes.c_int16(1)
g = "cyclic"
graph = g.encode('utf-8')
check_first = ctypes.c_bool(True)

x = index.quantum_table_index(Ns,playerpos1,playerpos2,graph,check_first)

print(x)