import numpy
import math
import os

def skip_lines(f,lines):
    for i in range(lines):
        f.readline()

def get_data(fileName):

    numberOfModels = 10
    #hold the positions - FIX THIS LATER
    #statesInRbt = [[[],[]],[[],[]],[[],[]],[[],[]],[[],[]],[[],[]]]
    statesInRbt = []
    #for i in range(numberOfModels):
    #    statesInRbt.append([[],[]])
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
    currentModel = -1
    # here's where we need to get the cartesian version of the states
    for i in range(numStates):
        holdLine = f.readline()
        valList = holdLine.split(',')
        numList = []
        numList.append(int(valList[0]))
        numList.append(float(valList[1]))
        numList.append(float(valList[2]))
        # this is a huge hack
        # this will only work if we have models changing
        # can't have two different model-params with the same model in a row
        #print currentModel
        #print numList[0]
        
        if currentModel == numList[0]:
            #print "true"
            statesInRbt[-1][0].append(numList[1])
            statesInRbt[-1][1].append(numList[2])
        else:
            statesInRbt.append([[],[]])
            currentModel = numList[0]
            statesInRbt[-1][0].append(numList[1])
            statesInRbt[-1][1].append(numList[2])

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
    logProbs_T = [[]]
    logProbs_O = [[]]
    actions = [[],[]]
    obs = [[],[]]
    print len(logProbs)
    for i in range(numSteps):
        skip_lines(f,5)
        print len(logProbs)

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

        # T
        skip_lines(f,1)
        
        logProbs_T.append([])
        for i in range(numStates):
            logProbs_T[-1].append(float(f.readline()))

        # O
        skip_lines(f,1)
        print len(logProbs)
        logProbs_O.append([])
        for i in range(numStates):
            logProbs_O[-1].append(float(f.readline()))

        # total
        skip_lines(f,1)
        
        logProbs.append([])
        for i in range(numStates):
            logProbs[-1].append(float(f.readline()))

        print len(logProbs)
            
        skip_lines(f,1+numMPPairs+1)
        #skip_lines(f,11+numStates+1+numMPPairs+1)
        for j in range(numMPPairs):
            data[j].append(float(f.readline()))
    f.close()


    return data, numSteps, numMPPairs, model, statesInRbt, logProbs_T, logProbs_O, logProbs, poses, actions, obs, actionType, numVarTypes
