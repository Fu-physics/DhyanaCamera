
from instrumental import instrument, list_instruments
#from instrumental.drivers.cameras import uc480
from instrumental.drivers.cameras import uc480
import numpy as np
import time as Time
import matplotlib.pyplot as plt
import xarray as xr
import os
import time as Time
from PyQt5 import QtCore

from ctypes import *
import ctypes as ct
from numpy.ctypeslib import ndpointer
import matplotlib.pyplot as plt


''' 
    --------------------------------------------->   X (1280)
    |           
    |        ______Width________
    |       |                   |
    |       |                   |
    |      High                 |
    |       |                   |
    |       |___________________| 
    |

    Y

  (1024)

'''


class ThorlabsCamer():
    def __init__(self):
        self.mydll = cdll.LoadLibrary("C:\\Users\\Agarwal\\Desktop\\CameraGithub\\Fu_Camera\\CameraDLL_ReturnPointer\\x64\\Debug\\CameraDLL_Fu.dll")
        print(self.mydll)
        

    def OpenCamera(self):
        self.cam = self.mydll.OpenCamera() 
        print(self.cam)

    def CloseCamera(self):
        self.mydll.CloseCamera() 

    
    def SetROIMod(self, hOffset, vOffset, width, hight):
        self.mydll.SetROIMod.argtypes = (ct.c_int, ct.c_int, ct.c_int, ct.c_int)
        self.mydll.SetROIMod.restype = None
        self.hOffset = hOffset
        self.vOffset = vOffset
        self.width = width
        self.hight = hight

        self.mydll.SetROIMod(self.hOffset, self.vOffset, self.width, self.hight)

    def SingleImageData(self):
        self.mydll.CaptureSingleFrame.argtype = ct.c_int
        self.mydll.CaptureSingleFrame.restype = ndpointer(dtype=ct.c_ushort, shape=(self.hight, self.width))
        imageData = self.mydll.CaptureSingleFrame(2)
        #infoObjSingle.append("ERROR OCCURE !")
        return imageData

    def CaptureStop(self):
        self.mydll.CaptureStop()

    def BufRelease(self):
        self.mydll.BufRelease()



if __name__ == "__main__":
    cam = ThorlabsCamer()
    cam.OpenCamera()
 
    cam.SetROIMod(0,0,2048,2048)
    imageData = cam.SingleImageData()

    #cam.MultiImageData()

    plt.subplot(111)
    plt.imshow(imageData)
    #plt.colorbar()

    cam.CloseCamera()

    #plt.savefig('oneframe.eps', format='eps', dpi=300)

    plt.show()