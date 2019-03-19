from __future__ import unicode_literals
import sys
import os
import random
import time

import numpy as np
import xarray as xr


from PyQt5 import QtCore, QtWidgets, QtGui
from PyQt5.QtWidgets import QInputDialog, QPushButton, QMainWindow, QApplication, QSpinBox, QLabel
from PyQt5.QtWidgets import QWidget, QAction, QTabWidget, QVBoxLayout, QHBoxLayout
from PyQt5.QtWidgets import QGroupBox, QDialog, QGridLayout
from PyQt5.QtWidgets import QApplication, QWidget, QLineEdit, QFileDialog
from PyQt5.QtWidgets import QPlainTextEdit
from PyQt5.QtWidgets import QFileDialog


import matplotlib
from mpl_toolkits.axes_grid1 import make_axes_locatable
# Make sure that we are using QT5BB
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
#plt.ion()
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib import cm


from scipy.stats import poisson
from statistics import mean
import seaborn as sns

import sys


#home made packages
from ThorlabsCamera import ThorlabsCamer

from DataProcess import DataProcess

#from DataProcess import ReadData

#from MatplotlibEmbeddingQt5 import MyMplCanvas, MyStaticMplCanvas, MyDynamicMplCanvas

#Gui package

from test1 import Ui_MainWindow



import pyqtgraph as pg


progname = os.path.basename(sys.argv[0])
progversion = "0.1"


class MyUi(QMainWindow, Ui_MainWindow):
    def __init__(self, parent = None):
        super (MyUi, self).__init__(parent)
        self.setupUi(self)


###    Table 1 ###########################################################################
        self.pushButton_ConnectCamera.clicked.connect(self.ConnectCamera)
        self.pushButton_SetCamera.clicked.connect(self.SetCamera)

        ## Camera imaging plot
        self.plotImaging = plt.figure("Imaging")
        axes_SingImaging = self.plotImaging.add_subplot(111)
        self.add_plotfigure(self.plotImaging, self.verticalLayout_ImageShow)
        ## Start and Stop button
        self.pushButton_StartImaging.clicked.connect(self.StartImaging(axes_SingImaging))
        self.pushButton_StopImaging.clicked.connect(self.StopImaging)



    def ConnectCamera(self):
        """
        connect the camera

        :return:
        """
        #try:
        self.Cam = ThorlabsCamer()
        self.Cam.OpenCamera()


    def SetCamera(self):

        exposeTime = self.spinBox_ExpTime.value()
        width = self.spinBox_Width.value()
        xshift = self.spinBox_XShift.value()
        hight = self.spinBox_Hight.value()
        yshift = self.spinBox_Yshift.value()

        if (xshift - 16 )%4 == 0 and (yshift - 16 )%4 == 0 and width%4 == 0 and hight%4 == 0:
            self.Cam.SetROIMod(hOffset=xshift, vOffset=yshift, width=width, hight=hight)
            print("setting the camera ...")

        else:
            print("Wrong setting parameter ...")

            
    def add_plotfigure(self, figureName, plot_layout):
        # self.figureName = plt.figure()                      # a figure instance to plot on
        # if put "plt.ion" on the head, which will make two more figures idependently.

        # this is the Canvas Widget that displays the `figure`, it takes the `figure` instance as a parameter to __init__
        canvas_figureName = FigureCanvas(figureName)
        toolbar_figureName = NavigationToolbar(canvas_figureName,
                                               self)  # this is the Navigation widget, it takes the Canvas widget and a parent

        plot_layout.addWidget(toolbar_figureName)  # this also needed to show the Navigation of plot
        plot_layout.addWidget(canvas_figureName)  # add Canvas Widget(plot widget) onto tab_2

    
    def StartImaging(self, ax):

        """
        1) get each frame and plot it
        2) auto-update the plot

        :param ax: plt.addsubplot(111)
        :return: auto-update the plot
        """

        def inner_SingleImage_fun():

            def update_data():
                #print("AutoFresh  the figure !")

                #print("begin to get SingleImageData data ... ")
                singledata = self.Cam.SingleImageData()
                #release the buffer after you got the image data
                self.Cam.CaptureStop()
                self.Cam.BufRelease()
                #print("The image data shape is: ", np.shape(singledata))
                cax.set_data(singledata)

                #We need to draw *and* flush
                self.plotImaging.canvas.draw()
                self.plotImaging.canvas.flush_events()

            self.timer = QtCore.QTimer(self)
            #print("begin to plot !")
            self.textBrowser_SetMeasureInf.setTextColor(QtCore.Qt.green)
            self.textBrowser_SetMeasureInf.append("Start Imaging . . .")

            try:
                self.cbar.remove()
                #print("clear self.cbar !")
            except:
                pass
                #print("fail to clear self.cbar !")

            ax.cla()
            singledata = self.Cam.SingleImageData()
            #release the buffer after you got the image data
            self.Cam.CaptureStop()
            self.Cam.BufRelease()
            cax = ax.imshow(singledata, interpolation='nearest')
            ax.set_title('CMOS Camera')
            self.cbar = self.plotImaging.colorbar(cax, orientation='vertical')

            #cbar.ax.set_xticklabels(['Low', 'Medium', 'High'])  # horizontal colorbar
            self.timer.timeout.connect(update_data)
            self.timer.start(100)

        return inner_SingleImage_fun


    def StopImaging(self):
        """
        Stop the auto-plot
        """
        self.timer.stop()
        self.textBrowser_SetMeasureInf.setTextColor(QtCore.Qt.red)
        self.textBrowser_SetMeasureInf.append("Stop Imaging!")
        self.Cam.CloseCamera()


if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    ui = MyUi()
    ui.show()
    sys.exit(app.exec_())

