import ctypes

playermove3 = ctypes.CDLL('C:/Users/jt565/Desktop/PHD/rendezvoususingC/PlayerMovement/playermove.so', winmode=0)

playermove3.main()