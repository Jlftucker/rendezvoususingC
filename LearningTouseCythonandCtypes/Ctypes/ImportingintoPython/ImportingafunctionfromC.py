import ctypes # import types
import time
#make a wrapper for the shared library

adderlib = ctypes.CDLL('C:/Users/jt565/Desktop/PHD/LearningTouseCythonandCtypes/Ctypes/ImportingintoPython/adder.so', winmode=0)

t1 = time.now()
print(t1)
print(adderlib.adder(1,2))