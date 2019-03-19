import numpy as np
import time as Time
import xarray as xr

from scipy.stats import poisson
from statistics import mean


import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib import cm
import seaborn as sns

import os


class DataProcess():
    def __init__(self, filename = 'test.nc'):
        ds_disk = xr.open_dataset(filename)

        try :
            self.frames = ds_disk.attrs["frameNumber"]
            self.hight = ds_disk.attrs["hight"]
            self.width = ds_disk.attrs["width"]
            self.note = ds_disk.attrs["note"]
        except Exception:
            print("Old *.nc file")

        self.image = np.array(ds_disk["CameraMatrix"])

        try :
            self.frames, self.hight, self.width  = self.image.shape
            self.note = ds_disk.attrs["note"]
        except Exception:
            print("fail to read the experiment data ! ")


        print("the shape of data is {}".format(self.image.shape))
        print("note is {}".format(self.note))
    
    def PixelData(self, y_location, x_location):

        return self.image[:, y_location, x_location]


    def Average_Fluctuation(self):
        """
        1. calculate the Average intensity, which is the first order imaging
        2. calculate the Fluctuation

        return average

        """
        matrix3d = self.image
        frame, height, width = matrix3d.shape
        print("the image size (frame, width, height) is:", frame, height, width)
        average = np.ones([height, width])

        # get the average imaging 
        for i in range(height):
            for j in range(width):
                average[i,j] =  np.sum(matrix3d[:,i,j])
        average = average/frame

        # get fluctuation
        self.fluctuation = np.zeros(np.shape(self.image))
        for i in range(self.frames):
            self.fluctuation[i] =  self.image[i] - average

        return average

        
    def BoseEinstein(self, Nbar, n = 51):
        nList = np.linspace(0, n, n+1, dtype = int)
        result = 1/(1+Nbar)*(Nbar/(1+Nbar))**nList
        return result


    def G2(self, FirstArray1d, SecondArray1d):

        Frames = len(FirstArray1d)
        #print("Frames is: " , Frames)
        FirstAverage = np.sum(FirstArray1d)/Frames
        SecondAverage = np.sum(SecondArray1d)/Frames
        
        result = np.sum(FirstArray1d*SecondArray1d)/ Frames /(FirstAverage * SecondAverage)
        return result
    
    
    def SpatialCorrelation(self, originPoint):
        
        x = originPoint[0]
        y = originPoint[1]
        
        ArrayOrigin = self.image[:, y, x]
        
        frame, height, width  = self.image.shape
        
        
        ##  the X direction correlation  
        xCorrelation = np.zeros(width)
        for i in range(width):
            xCorrelation[i] =  self.G2(ArrayOrigin, self.image[:, y, i])
        
        ##  the Y direction correlation  
        yCorrelation = np.zeros(height)
        for i in range(height):
            yCorrelation[i] =  self.G2(ArrayOrigin, self.image[:, i, x])

        return xCorrelation, yCorrelation


    def PlotPhotonDistribution(self):

        """
        Plot the photon number distribution ...
        """
                
        font = {'family': 'serif',
                'color':  'darkred',
                'weight': 'normal',
                'size': 16}

        Nmax = 100

        bins = np.linspace(0, Nmax, Nmax+1)

        n = Nmax
        nList = np.linspace(0, n, n+1, dtype = int)

        y_location = 20
        x_location = [0, 25]

        j = 0

        for i in x_location:
            
            Array1 = self.image[:, y_location, i]
            Array2 = self.image[:, y_location, i]
            g2 = self.G2(Array1, Array2)
            print("g2 is:", g2)

            arr = []
            rv = poisson(self.firstOrdImaging[y_location, i])
            for num in range(0,40):
                arr.append(rv.pmf(num))
            j = j + 1
            fig, ax = plt.subplots()
            
            ax.hist(self.image[:, y_location, i] , bins, normed=True, label = "Data distribution") 
            ax.plot(nList, self.BoseEinstein(self.firstOrdImaging[y_location, i], n), label ="BoseEinstein distribution")
            ax.plot(arr, linewidth=2.0, label ="Possion distribution")
            ax.set_title("Pixel Position({},{}); <$I$>:{}".format(i , y_location, self.firstOrdImaging[y_location, i]), fontdict=font)
            
            ax.text(22, .08, r"g2:{}".format(g2), fontdict=font)
            ax.legend() 
            
            fig.savefig('PixelPosition({},{})PhotDist.eps'.format(i , y_location), format='eps', dpi=300)


    def SecondOrder(self):
        """
        return the second Order imaging ...
        Plot the second Order imaing ...
        """

        secondOrderFluc = self.fluctuation * self.fluctuation

        secondOrderImaging = np.ones([self.hight, self.width])
        for i in range(self.hight):
            for j in range(self.width):
                secondOrderImaging[i,j] =  np.sum(secondOrderFluc[:,i,j])
        secondOrderImaging = secondOrderImaging/self.frames

        return secondOrderImaging

    def NOrder(self, Norder = 2):
        """
        return the second Order imaging ...
        Plot the second Order imaing ...
        """
        NOrderFluc = np.ones(np.shape(self.image))

        for i in range(Norder):
            NOrderFluc = NOrderFluc * self.fluctuation

        NOrderImaging = np.ones([self.hight, self.width])
        for i in range(self.hight):
            for j in range(self.width):
                NOrderImaging[i,j] =  np.sum(NOrderFluc[:,i,j])
        NOrderImaging = NOrderImaging / self.frames
        
        del NOrderFluc

        return NOrderImaging


    def PlotFirstOdr(self):


        self.firstOrdImaging = self.Average_Fluctuation()

        fig = plt.figure()

        # plot the Averge imaging
        ax = fig.add_subplot(211)
        im = ax.imshow(self.firstOrdImaging)
        # create an axes on the right side of ax. The width of cax will be 5%
        # of ax and the padding between cax and ax will be fixed at 0.05 inch.
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.05)
        plt.colorbar(im, cax=cax)
        #plt.colorbar(im, cax=cax, ticks=[0, 5, 10])
        ax.set_title('1th Order')

        ax1 = fig.add_subplot(212)
        ax1.plot(self.firstOrdImaging[25,:])
        ax1.set_title('1th Order @y=25')

        plt.tight_layout()

        fig.savefig('1th Order Imaing.eps', format='eps', dpi=100)


    def PlotSecondOdr(self):

        secondOrdImaing = self.SecondOrder()

        ## plot the second order imaging
        fig = plt.figure()

        ax = fig.add_subplot(211)
        im = ax.imshow(secondOrdImaing)
        # create an axes on the right side of ax. The width of cax will be 5%
        # of ax and the padding between cax and ax will be fixed at 0.05 inch.
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.05)
        plt.colorbar(im, cax=cax)
        #plt.colorbar(im, cax=cax, ticks=[0, 50, 100])
        ax.set_title('2th Order')

        ax1 = fig.add_subplot(212)
        ax1.plot(secondOrdImaing[25,:])
        ax1.set_title('2th Order @y=25')

        plt.tight_layout()

        fig.savefig('2th Order Imaing.eps', format='eps', dpi=100)


    def PlotFrames(self):
        fig1, ax1 = plt.subplots()

        ax1.imshow(self.image[1,:,:])
        cax1 = ax1.imshow(self.image[1,:,:])
        ax1.set_title("The #1 frame")

        # Add colorbar, make sure to specify tick locations to match desired ticklabels
        fig1.colorbar(cax1)
        #cbar1 = fig1.colorbar(cax1, ticks=[-1, 0, 1])
        #cbar1.ax1.set_yticklabels(['< -1', '0', '> 1'])  # vertically oriented colorbar
        fig1.savefig('The #1 frame.eps', format='eps', dpi=100)


        fig2, ax2 = plt.subplots()
        ax2.imshow(self.image[10,:,:])
        cax2 = ax2.imshow(self.image[1,:,:])
        ax2.set_title("The #10 frame")
        fig2.colorbar(cax2)
        #cbar2 = fig2.colorbar(cax2, ticks=[-1, 0, 1])
        #cbar1.ax1.set_yticklabels(['< -1', '0', '> 1'])  # vertically oriented colorbar
        fig2.savefig('The #10 frame.eps', format='eps', dpi=100)


if __name__ == "__main__":
    app = DataProcess()   # which read the data only
    app.PlotFrames()

    # calculated the first order which will be used at photon distribution, high order imaging, so do it firstly.
    # and plot it
    app.PlotFirstOdr()    


    app.PlotPhotonDistribution()

    app.PlotSecondOdr()


