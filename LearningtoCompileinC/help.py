import ctypes

testlibrary = ctypes.CDLL('libfirst.so', winmode=0)

testlibrary.main()