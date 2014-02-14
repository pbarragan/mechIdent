from matplotlib import pyplot
from matplotlib import colors
import numpy
import math

def skip_lines(f,lines):
    for i in range(lines):
        f.readline()

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
#fileName = 'data/data0Thu_Dec_12_09_30_05_2013.txt' #relative but wrong
#fileName = 'data/data0Thu_Dec_12_14_10_47_2013.txt' #absolute but wrong

#fileName = 'data/data0Thu_Dec_12_09_30_05_2013.txt' #relative but wrong
#fileName = 'data/data0Sun_Dec_15_23_19_13_2013.txt' #absolute
#fileName = 'data/data0Sun_Dec_15_23_26_33_2013.txt' #absolute 10 steps
#fileName = 'data/data0Sun_Dec_15_23_42_44_2013.txt' #absolute with addition


#fileName = 'data/data0Sun_Dec_15_23_53_49_2013.txt' #absolute
#fileName = 'data/data0Mon_Dec_16_00_01_26_2013.txt' #relative

#consistent actions
#consistent transition model and real world
#trans var - 0.0001, obs var - 0.01
#fileName = 'data/data0Mon_Dec_16_00_03_35_2013.txt' #TM-relative, W-relative
#fileName = 'data/data0Mon_Dec_16_00_05_45_2013.txt' #TM-absolute, W-absolute
#fileName = 'data/data0Mon_Dec_16_00_53_28_2013.txt' #TM-absolute, W-relative
#fileName = 'data/data0Mon_Dec_16_00_57_23_2013.txt' #TM-relative, W-absolute

#trans var - 0.01, obs var - 0.01
#fileName = 'data/data0Mon_Dec_16_09_35_38_2013.txt' #TM-relative, W-relative
#fileName = 'data/data0Mon_Dec_16_09_32_21_2013.txt' #TM-absolute, W-absolute
#fileName = 'data/data0Mon_Dec_16_09_34_39_2013.txt' #TM-absolute, W-relative
#fileName = 'data/data0Mon_Dec_16_09_29_57_2013.txt' #TM-relative, W-absolute

#after fix
#trans var - 0.0001, obs var - 0.0001
#fileName = 'data/data0Mon_Dec_16_10_38_07_2013.txt' #TM-relative, W-relative
#fileName = 'data/data0Mon_Dec_16_10_35_14_2013.txt' #TM-absolute, W-absolute
#fileName = 'data/data0Mon_Dec_16_10_32_25_2013.txt' #TM-absolute, W-relative
#fileName = 'data/data0Mon_Dec_16_10_36_40_2013.txt' #TM-relative, W-absolute

#fileName = 'data/data0Thu_Feb__6_17_01_53_2014.txt' # just a test
#fileName = 'data/data0Wed_Feb_12_11_59_09_2014.txt' # testing modelParamPairs
#fileName = 'data/data0Wed_Feb_12_16_04_52_2014.txt' # back to more models
#fileName = 'data/data0Wed_Feb_12_16_12_25_2014.txt' # full models
#fileName = 'data/data0Wed_Feb_12_16_19_32_2014.txt' #full models, absolute
#fileName = 'data/data0Wed_Feb_12_16_28_02_2014.txt' # full models, absolute, regular actions
fileName = 'data/data0Thu_Feb_13_13_54_20_2014.txt' # full models, absolute, regular actions, noise, entropy action selection

data, numSteps, numMPPairs, model, statesInRbt, logProbs, poses, actions, obs, actionType, numVarTypes = get_data(fileName)

# make indeces from numVarTypes
probInds=[0]
for i in range(len(numVarTypes)):
    probInds.append(probInds[i]+numVarTypes[i])

# make list of symbols
symList = ["o","s","*","D","^","v"]

# model names list
modelNames = ["Free","Fixed","Rev","Pris","L1","L2"]

print probInds
print numVarTypes
#print len(logProbs[0][numVarTypes[0]:numVarTypes[1]])
print len(data)
print 'max'
print math.exp(max(logProbs[2]))
# plot parameters
mSize = 10
sSize = 50

if actionType == 0:
    actionString = 'Absolute'
elif actionType == 1:
    actionString = 'Relative'

rows = int(math.floor(math.sqrt(numSteps)))
columns = int(math.ceil(float(numSteps)/float(rows)))
if rows*columns<numSteps+1:
    if rows<columns:
        rows += 1
    else:
        columns += 1



# initial distribution
pyplot.subplot(rows,columns,1)
pyplot.subplots_adjust(left=0.05,right=1.0,bottom=0.15,top=0.95,wspace=0.2,hspace=0.4)
colorNorm = colors.Normalize(0,1) # YOU NEED THIS SO STUFF DOESN'T EXPLODDDEEE
# without colorNorm, you can't do repeated plotting and get a useful
# color answer becasue it normalize colors by default

'''
xs = []
ys = []
for j in range(len(statesInRbt)):
    xs.extend(statesInRbt[j][0])
    ys.extend(statesInRbt[j][1])
    
pyplot.scatter(xs,ys,c=logProbs[0],s=sSize)
'''
print 'hello'
print len(statesInRbt[1][0])
#print len(logProbs[0][numVarTypes[1]:numVarTypes[2]])
print len(data[0])
for j in range(len(statesInRbt)):
    pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs[0][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)

# create the list for the legend
legendList=[]
for i in range(len(data)): 
    legendList.append(modelNames[i]+" - "+str("%.3G" % (data[i][0])))
    
pyplot.legend(legendList,bbox_to_anchor=(0., -.325, 1., .5), loc=3, ncol=2, mode="expand", borderaxespad=0.)
pyplot.colorbar()
pyplot.plot(poses[0][0],poses[1][0],'ro',markersize=mSize)
pyplot.title('initial - '+actionString)

for i in range(numSteps):
    pyplot.subplot(rows,columns,i+2)
    pyplot.clim(0,1)

    
    for j in range(len(statesInRbt)):
        pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs[i+1][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)
        pyplot.hold(True)
        #print 'max'+str(i)+str(j)
        #print math.exp(max(logProbs[i+1][probInds[j]:probInds[j+1]]))
    
    '''
    xs = []
    ys = []
    for j in range(len(statesInRbt)):
        xs.extend(statesInRbt[j][0])
        ys.extend(statesInRbt[j][1])
    
    pyplot.scatter(xs,ys,c=[math.exp(x) for x in logProbs[i+1]],s=sSize)
    '''
    # create the list for the legend
    legendList=[]
    for k in range(len(data)): 
        legendList.append(modelNames[k]+" - "+str("%.3G" % (data[k][i+1])))
    
    pyplot.legend(legendList,bbox_to_anchor=(0., -.325, 1., .5), loc=3, ncol=2, mode="expand", borderaxespad=0.)
    pyplot.colorbar()
    pyplot.plot(poses[0][i],poses[1][i],'ro',markersize=mSize)
    pyplot.plot(poses[0][i+1],poses[1][i+1],'bo',markersize=mSize)

    if actionType == 0:
        pyplot.plot(actions[0][i],actions[1][i],'g^',markersize=mSize)
    elif actionType == 1:
        pyplot.plot(actions[0][i]+poses[0][i],actions[1][i]+poses[1][i],'g^',markersize=mSize)

    pyplot.plot(obs[0][i],obs[1][i],'yx',markersize=mSize)
    pyplot.title('step'+str(i))

pyplot.show()


pyplot.plot(statesInRbt[5][0],statesInRbt[5][1],'o')
pyplot.show()

'''
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
'''
