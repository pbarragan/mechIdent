from matplotlib import pyplot
from matplotlib import colors
import numpy
import math
import os

import readData

#fileName = 'data/data5Sun_May_25_10_27_57_2014.txt' # error
fileName = 'data/data0Tue_May_27_13_56_45_2014.txt' # model0

# parameters
plotSet = False

data, numSteps, numMPPairs, model, statesInRbt, logProbs_T, logProbs_O, logProbs, poses, actions, obs, actionType, numVarTypes = readData.get_data(fileName)

# make a folder to house stuff
folderName = fileName[:fileName.find('.txt')].replace('/','-')+'-BAR'
setPath = 'dump/'+folderName+'/'

if not os.path.exists(setPath):
    os.mkdir(setPath)

# model names list
modelNames = ["Free","Fixed","Rev I","Pris I","L1 I","L2 I","Rev II","Pris II","L1 II","L2 II"]

inds = range(numSteps+1)
numGroups = len(modelNames)
index = numpy.arange(numGroups)
bar_width = 0.50
totalData = numpy.array(data)

for i in range(numSteps):
    pyplot.bar(index,totalData[:,i],bar_width,align='center')
    pyplot.xlabel('Model',size=13)
    pyplot.ylabel('Probability',size=13)
    pyplot.xticks(index,("Free","Fixed","Rev I","Pris I","L1 I","L2 I","Rev II","Pris II","L1 II","L2 II"),size=13)
    pyplot.ylim((0,1))
    pyplot.savefig(setPath+'step'+str(i)+'.png')
    pyplot.clf()
    #if i == 0:
    #pyplot.show()
    #else:
    #pyplot.draw()  
    
    
