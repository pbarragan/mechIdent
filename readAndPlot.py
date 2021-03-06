from matplotlib import pyplot
import numpy

def skip_lines(f,lines):
    for i in range(lines):
        f.readline()

def get_data(fileName):
    f = open(fileName,'r')
    #read the initial stuff you need
    f.readline()
    model = int(f.readline())
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
model = 0
asTypes = ['simple','random','entropy']
asNum = 2

path = 'data/model'+str(model)+'/'+asTypes[asNum]+'/'
print path
startFileName = path+'data'+str(model)+'_0.txt'


#setup
startData, nSteps, nMPPairs, m = get_data(startFileName)
totalData = numpy.array(startData)

trials = 10 #how many trials

for i in range(1,trials):
    fileName = path+'data'+str(model)+'_'+str(i)+'.txt'
    data, numSteps, numMPPairs, model = get_data(fileName)
    dataArray = numpy.array(data)
    totalData += dataArray

avgData = totalData/trials
#print avgData[5,:]

inds = range(nSteps+1)
models = ['Free','Fixed','Rev','Pris','RevPrisL','PrisPrisL']
for i in range(nMPPairs):
    pyplot.plot(inds,avgData[i,:],'-o')

pyplot.title(models[m]+' - '+asTypes[asNum])
pyplot.ylabel('Probability')
pyplot.xlabel('Step')
pyplot.legend(models,loc=2)
pyplot.show()


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

