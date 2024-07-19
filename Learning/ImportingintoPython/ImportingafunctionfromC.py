import ctypes # import types
import time
#make a wrapper for the shared library

adderlib = ctypes.CDLL('adder.so', winmode=0)


print(adderlib.adder(6,7))