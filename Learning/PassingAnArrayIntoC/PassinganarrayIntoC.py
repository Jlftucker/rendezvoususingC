'''
File to pass an array into C since we are having issues with it 

'''
import ctypes
import numpy as np

# Load the shared library
lib = ctypes.CDLL('C:/Users/jt565/Desktop/PHD/rendezvoususingC/Learning/PassingAnArrayIntoC/mylib.dll', winmode=0)  # For Linux
# lib = ctypes.CDLL('./libexample.dll')  # For Windows
# lib = ctypes.CDLL('./libexample.dylib')  # For macOS

# Define the argument and return types
lib.print_array.argtypes = (ctypes.POINTER(ctypes.c_int), ctypes.c_int)
lib.print_array.restype = ctypes.c_int

# Create a NumPy array
numpy_array = np.array([1, 2, 3, 4, 5], dtype=np.int32)

# Ensure that the array is contiguous (C-style)
if not numpy_array.flags['C_CONTIGUOUS']:
    numpy_array = np.ascontiguousarray(numpy_array, dtype=np.int32)

# Pass the NumPy array to the C function
x = lib.print_array(numpy_array.ctypes.data_as(ctypes.POINTER(ctypes.c_int)), len(numpy_array))


#So it should return 6 but it seems to return 10 for some reason??? 

#Apparently this is because of the file either being an so or it doesn't understand where the pointer is going so it says a random number on the stack

print(x)