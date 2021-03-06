#import matplotlib
from matplotlib import pyplot
import numpy

def skip_lines(f,lines):
    for i in range(lines):
        f.readline()

def get_data(fileName):
    f = open(fileName,'r')
    #read the initial stuff you need
    f.readline()
    model = f.readline()
    skip_lines(f,5)
    numStates = int(f.readline())
    skip_lines(f,numStates*6+2)
    numActions = int(f.readline())
    skip_lines(f,numActions+2)
    numMPPairs = int(f.readline())
    skip_lines(f,numMPPairs*(6+1)+(1+1+1+1)+numStates)
    data = []
    for i in range(numMPPairs):
        data.append([float(f.readline())])
    skip_lines(f,3)
    numSteps = int(f.readline())
        
    #this has to happen until you get to the end of the file
    for i in range(numSteps):
        skip_lines(f,11+numStates+1+numMPPairs+1)
        for j in range(numMPPairs):
            data[j].append(float(f.readline()))
    f.close()

    print model
    print numStates
    print numActions
    print numMPPairs
    print numSteps
    print len(data)
    print len(data[0])
    return data, numSteps, numMPPairs, model

def get_data_w_actions(fileName):
    f = open(fileName,'r')
    #read the initial stuff you need
    f.readline()
    model = f.readline()
    skip_lines(f,5)
    numStates = int(f.readline())
    skip_lines(f,numStates*6+2)
    numActions = int(f.readline())
    skip_lines(f,numActions+2)
    numMPPairs = int(f.readline())
    skip_lines(f,numMPPairs*(6+1)+(1+1+1+1)+numStates)
    data = []
    for i in range(numMPPairs):
        data.append([float(f.readline())])
    skip_lines(f,3)
    numSteps = int(f.readline())
        
    #this has to happen until you get to the end of the file
    for i in range(numSteps):
        skip_lines(f,5)
        actionString = f.readline()
        print actionString
        skip_lines(f,5)
        skip_lines(f,numStates+1+numMPPairs+1)
        for j in range(numMPPairs):
            data[j].append(float(f.readline()))
    f.close()

    #print model
    #print numStates
    #print numActions
    #print numMPPairs
    #print numSteps
    #print len(data)
    #print len(data[0])
    return data, numSteps, numMPPairs, model
    
#main
#just to get the numbers we want
#matplotlib.rcParams.update({'font.size': 22})

model = 5
asTypes = ['simple','random','entropy']
asNum = 2
trialNum = 1

path = 'data/model'+str(model)+'/'+asTypes[asNum]+'/'

startFileName = path+'data'+str(model)+'_'+str(trialNum)+'.txt'

startFileName = 'data/videoData/data0.txt'

#setup
startData, nSteps, nMPPairs, m = get_data_w_actions(startFileName)
totalData = numpy.array(startData)

m = 0

inds = range(nSteps+1)
models = ['Free','Fixed','Rev','Pris','RevPrisL','PrisPrisL']
numGroups = len(models)
index = numpy.arange(numGroups)
bar_width = 0.70

for i in range(10):
    pyplot.bar(index,totalData[:,i],bar_width,align='center')
    pyplot.xlabel('Model',size=13)
    pyplot.ylabel('Probability',size=13)
    pyplot.xticks(index,('Free','Fixed','Rev','Pris','RevPrisL','PrisPrisL'),size=13)
    pyplot.ylim((0,1))
    pyplot.savefig('data/videoData/figures/model'+str(m)+'/step'+str(i)+'.png')
    pyplot.clf()
    #if i == 0:
        #pyplot.show()
    #else:
        #pyplot.draw()
        

print 'hello'

#pyplot.title(models[m]+' - '+asTypes[asNum])
#pyplot.ylabel('Probability')
#pyplot.xlabel('Step')
#pyplot.legend(models)
#pyplot.show()


## data, numSteps, numMPPairs, model = get_data('data5Sun_Sep_15_17_10_07_2013.txt')

## inds = range(numSteps+1)
## models = ['Free','Fixed','Rev','Pris','RevPrisL','PrisPrisL']
## #colors = ['r','g',
## for i in range(numMPPairs):
##     pyplot.plot(inds,data[i],'-o')

## pyplot.title(models[model])
## pyplot.ylabel('Probability')
## pyplot.xlabel('Step')
## pyplot.legend(models)

## dataArray = numpy.array(data)
## print dataArray
## print dataArray[5,:]
## pyplot.show()

