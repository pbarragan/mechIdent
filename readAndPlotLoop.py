from matplotlib import pyplot
import numpy
from matplotlib import rc

import heapq # for max sorting

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

    numberOfModels = 10
    #hold the positions - FIX THIS LATER
    #statesInRbt = [[[],[]],[[],[]],[[],[]],[[],[]],[[],[]],[[],[]]]
    statesInRbt = []
    for i in range(numberOfModels):
        statesInRbt.append([[],[]])
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
    skip_lines(f,numStates*6+2) # 6 is correct. it's not about the model number.
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
            hi = f.readline()
            logProbs[-1].append(float(hi))
            
        skip_lines(f,1+numMPPairs+1)
        #skip_lines(f,11+numStates+1+numMPPairs+1)
        for j in range(numMPPairs):
            data[j].append(float(f.readline()))

    times = []
    for i in range(3):
        f.readline()
        times.append(float(f.readline()))
    
    f.close()


    return data, numSteps, numMPPairs, model, statesInRbt, logProbs, poses, actions, obs, actionType, numVarTypes, times

'''
fileTemp = 'data/data0Wed_Apr__9_14_41_16_2014.txt'
startData, nSteps, nMPPairs, m, sInRbt, logPs, Ps, As, Os, aType, numVTypes = get_data(fileTemp)

print startData

raise Exception("I know python!")
'''



#
##
###
####
###
##
#
















#main
#just to get the numbers we want
#model = 0
modelNums = [0,1,2,3,4,5,6,7,8,9]
asTypes = ['simple','random','entropy','OG']
#asNum = 2
asNums = [1,2,3] # needs 1,2
outPath = 'dataBigBoth/'
inPath = outPath
models = ['Free','Fixed','Rev','Pris','RevPrisL','PrisPrisL','Rev2','Pris2','RevPrisL2','PrisPrisL2']

outputFile = outPath+'modelSelections.txt'
fout = open(outputFile,'w')

statsFile = outPath+'stats.txt'
statsOut = open(statsFile,'w')

#statsHeader = 'Real Model:\tAction Selection Type:\tFirst:\tP:\tSecond:\tP:\tThird:\tP:\n'
statsHeader = 'M\tA\tF\tProbability\tS\tProbability\tT\tProbability\n'
statsOut.write(statsHeader)

errorCount = 0

avgStepTimes = [[],[],[]]

for modelNum in modelNums:
    for asNum in asNums:
        path = inPath+'model'+str(modelNum)+'/'+asTypes[asNum]+'/'
        # print path
        startFileName = path+'data'+str(modelNum)+'_0.txt'
        print startFileName


        # setup
        startData, nSteps, nMPPairs, m, sInRbt, logPs, Ps, As, Os, aType, numVTypes, startTimes = get_data(startFileName)
        totalData = numpy.array(startData)

        trials = 10 #how many trials

        totalStepTime=startTimes[2] # get the step time for the first experiment

        for i in range(1,trials):
            fileName = path+'data'+str(modelNum)+'_'+str(i)+'.txt'
            data, numSteps, numMPPairs, model, statesInRbt, logProbs, poses, actions, obs, actionType, numVarTypes, times = get_data(fileName)
            maxes=[]
            for f in range(len(data)):
                maxes.append(data[f][-1])

            top3 = heapq.nlargest(3,maxes)
            infoString = 'model: '+str(modelNum)+', best: '+str(maxes.index(max(maxes)))+', AStype: '+str(asNum)+', top: '+str(maxes.index(top3[0]))+', p= '+str(top3[0])+', second: '+str(maxes.index(top3[1]))+', p= '+str(top3[1])+', third: '+str(maxes.index(top3[2]))+', p= '+str(top3[2])+'\n'
            #print infoString
            fout.write(infoString)
            dataArray = numpy.array(data)
            totalData += dataArray
            if modelNum != maxes.index(top3[0]):
                errorCount += 1
            totalStepTime += times[2]

        avgData = totalData/trials
        avgStepTime = totalStepTime/trials

        avgStepTimes[asNum-1].append(avgStepTime)

        # print avgData[5,:]
        

        # write a file with averages for later
        #print avgData
        #print avgData[:,-1]
        lastColumn = avgData[:,-1].tolist()
        top3ofLC = heapq.nlargest(3,lastColumn)
        
        statsString = str(modelNum)+'\t'+str(asNum)+'\t'+str(lastColumn.index(top3ofLC[0]))+'\t'+'%.7f'%top3ofLC[0]+'\t'+str(lastColumn.index(top3ofLC[1]))+'\t'+'%.7f'%top3ofLC[1]+'\t'+str(lastColumn.index(top3ofLC[2]))+'\t'+'%.7f'%top3ofLC[2]+'\n'
        statsOut.write(statsString)

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

statsOut.write('Total Errors: '+str(errorCount))

fout.close()
statsOut.close()

for i in range(3):
    print sum(avgStepTimes[i])/len(avgStepTimes[i])


