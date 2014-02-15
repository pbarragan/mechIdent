from matplotlib import pyplot
import numpy
from matplotlib import rc

font = {'size':16}
rc('font',**font)

def skip_lines(f,lines):
    for i in range(lines):
        f.readline()

'''
def get_data(fileName):
    f = open(fileName,'r')
    #read the initial stuff you need
    f.readline()
    actionType = int(f.readline())
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
'''

def get_data(fileName):

    #hold the positions - FIX THIS LATER
    statesInRbt = [[[],[]],[[],[]],[[],[]],[[],[]],[[],[]],[[],[]]]
    #hold the log probs - FIX THIS LATER
    logProbs = [[]]
    
    f = open(fileName,'r')
    #read the initial stuff you need
    f.readline()
    actionType = int(f.readline())
    f.readline()
    model = int(f.readline())
    skip_lines(f,5)
    numStates = int(f.readline())
    skip_lines(f,numStates*6+2)
    # here's where we need to get the cartesian version of the states
    for i in range(numStates):
        holdLine = f.readline()
        valList = holdLine.split(',')
        numList = []
        numList.append(int(valList[0]))
        numList.append(float(valList[1]))
        numList.append(float(valList[2]))
        statesInRbt[numList[0]][0].append(numList[1])
        statesInRbt[numList[0]][1].append(numList[2])
    skip_lines(f,1)  
    numActions = int(f.readline())
    skip_lines(f,numActions+2)
    numMPPairs = int(f.readline())
    # this is gonna be a problem
    numVarTypes = []
    skip_lines(f,1)
    for i in range(numMPPairs):
        skip_lines(f,5)
        numVarTypes.append(int(f.readline()))
    skip_lines(f,1)
    for i in range(numStates):
        logProbs[0].append(float(f.readline()))
        
    skip_lines(f,1+numMPPairs+1)

    # broken into two lines: skip_lines(f,numMPPairs*(6+1)+(1+1+1+1)+numStates)
    
    data = []
    for i in range(numMPPairs):
        data.append([float(f.readline())])
    skip_lines(f,1)

    # get the initial pose in rbt:
    poses = [[],[]]
    holdPose = f.readline()
    stringList = holdPose.split(',')
    stringList.pop() # get rid of the empty string at the end because of the last ,
    poseList = []
    poseList.append(float(stringList[0]))
    poseList.append(float(stringList[1]))
    poses[0].append(poseList[0])
    poses[1].append(poseList[1])
    
    skip_lines(f,1)
    numSteps = int(f.readline())
        
    #this has to happen until you get to the end of the file
    actions = [[],[]]
    obs = [[],[]]
    for i in range(numSteps):
        skip_lines(f,5)

        # get action. hellz yeah.
        holdAction = f.readline()
        actionString = holdAction.split(',')
        actionString.pop() # get rid of the empty string at the end because of the last ,
        actionList = []
        actionList.append(float(actionString[0]))
        actionList.append(float(actionString[1]))
        actions[0].append(actionList[0])
        actions[1].append(actionList[1])

        skip_lines(f,1)
        
        # get current pose
        holdP = f.readline()
        stringL = holdP.split(',')
        stringL.pop() # get rid of the empty string at the end because of the last ,
        poseL = []
        poseL.append(float(stringL[0]))
        poseL.append(float(stringL[1]))
        poses[0].append(poseL[0])
        poses[1].append(poseL[1])

        skip_lines(f,1)
        
        # get obs
        holdO = f.readline()
        stringLO = holdO.split(',')
        stringLO.pop() # get rid of the empty string at the end because of the last ,
        obsL = []
        obsL.append(float(stringLO[0]))
        obsL.append(float(stringLO[1]))
        obs[0].append(obsL[0])
        obs[1].append(obsL[1])

        skip_lines(f,1)
        
        logProbs.append([])
        for i in range(numStates):
            logProbs[-1].append(float(f.readline()))
            
        skip_lines(f,1+numMPPairs+1)
        #skip_lines(f,11+numStates+1+numMPPairs+1)
        for j in range(numMPPairs):
            data[j].append(float(f.readline()))
    f.close()


    return data, numSteps, numMPPairs, model, statesInRbt, logProbs, poses, actions, obs, actionType, numVarTypes

#main
#just to get the numbers we want
#model = 0
modelNums =[0,1,2,3,4,5]
asTypes = ['simple','random','entropy']
#asNum = 2
asNums = [1,2]
outPath = 'dataRewrite/'
models = ['Free','Fixed','Rev','Pris','RevPrisL','PrisPrisL']

for modelNum in modelNums:
    for asNum in asNums:
        path = 'dataRewrite/model'+str(modelNum)+'/'+asTypes[asNum]+'/'
        # print path
        startFileName = path+'data'+str(modelNum)+'_0.txt'


        # setup
        startData, nSteps, nMPPairs, m, sInRbt, logPs, Ps, As, Os, aType, numVTypes = get_data(startFileName)
        totalData = numpy.array(startData)

        trials = 10 #how many trials

        for i in range(1,trials):
            fileName = path+'data'+str(modelNum)+'_'+str(i)+'.txt'
            data, numSteps, numMPPairs, model, statesInRbt, logProbs, poses, actions, obs, actionType, numVarTypes = get_data(fileName)
            maxes=[]
            for f in range(len(data)):
                maxes.append(data[f][-1])
            print 'model: '+str(modelNum)+', best: '+str(maxes.index(max(maxes)))+', AStype: '+str(asNum)
            dataArray = numpy.array(data)
            totalData += dataArray

        avgData = totalData/trials
        # print avgData[5,:]

        inds = range(nSteps+1)
        for i in range(nMPPairs):
            pyplot.plot(inds,avgData[i,:],'-o')

        # pyplot.title(models[m]+' - '+asTypes[asNum])
        pyplot.ylabel('Probability')
        pyplot.xlabel('Step')
        pyplot.ylim(0,1)
        pyplot.xlim(0,nSteps)
        pyplot.legend(models,loc=0)
        outFile = outPath+'m'+str(model)+str(asTypes[asNum][0])+'.png'
        pyplot.savefig(outFile,bbox_inches='tight')
        pyplot.clf()


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

