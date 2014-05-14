from matplotlib import pyplot
from matplotlib import colors
import numpy
import math
import os

import readData

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
#fileName = 'data/data0Thu_Feb_13_13_54_20_2014.txt' # full models, absolute, regular actions, noise, entropy action selection
#fileName = 'data/data0Fri_Feb_14_14_55_50_2014.txt' # full models, absolute, regular actions, noise, entropy action selection, new state space for L2
#fileName = 'data/data0Fri_Feb_14_15_02_45_2014.txt' # try again
#fileName = 'data/data0Fri_Feb_14_15_40_07_2014.txt' # try again again - final

# 2/24/14 - seeing if absolute and relative are the same:
#fileName = 'data/data0Mon_Feb_24_12_37_16_2014.txt' # absolute
#fileName = 'data/data0Mon_Feb_24_12_38_29_2014.txt' # relative

# 2/26/14 - no noise
#fileName = 'data/data0Wed_Feb_26_09_40_55_2014.txt' # absolute
#fileName = 'data/data0Wed_Feb_26_09_41_34_2014.txt' # relative

#fileName = 'data/data0Thu_Feb_27_00_38_55_2014.txt' # random, relative
#fileName = 'data/data0Thu_Feb_27_00_40_47_2014.txt' # random, relative, 10 actions

#fileName = 'dataBig/model0/entropy/data0_0.txt' # big space
#fileName = 'data/data4Mon_Mar_17_15_01_43_2014.txt' # big space for checking states
#fileName = 'data/data0Mon_Mar_31_14_49_52_2014.txt' # big new state space
#fileName = 'data/data0Mon_Mar_31_15_39_34_2014.txt' # big new state space - fixed
#fileName = 'data/data0Mon_Mar_31_15_52_40_2014.txt' # big new state space - fixed actually
#fileName = 'data/data0Tue_Apr__1_10_39_52_2014.txt' # big new state space - fixed actually actually with more points for the rev pris latch.

#fileName = 'data/data0Tue_Apr__1_11_08_11_2014.txt' # testing the new big space
#fileName = 'data/data0Thu_Apr__3_14_09_44_2014.txt'
#fileName = 'dataBig/model0/entropy/data0_0.txt'

# interesting ones from most recent data before 4/15/14
#fileName = 'dataBigBigActions/model9/entropy/data9_4.txt'
#fileName = 'dataBigBoth/model5/entropy/data5_7.txt'
#fileName = 'dataBigBoth/model5/random/data5_5.txt'

# new format
fileName = 'data/data0Wed_May__7_13_21_13_2014.txt'

folderName = fileName[:fileName.find('.txt')].replace('/','-')
setPath = 'dump/'+folderName+'/'

if not os.path.exists(setPath):
    os.mkdir(setPath)

# parameters
plotLog = True
plotSet = False

data, numSteps, numMPPairs, model, statesInRbt, logProbs_T, logProbs_O, logProbs, poses, actions, obs, actionType, numVarTypes = readData.get_data(fileName)

# make indeces from numVarTypes
probInds=[0]
for i in range(len(numVarTypes)):
    probInds.append(probInds[i]+numVarTypes[i])

# make list of symbols
symList = ["o","s","*","D","^","v","x",".","<",">"]

# model names list
modelNames = ["Free","Fixed","Rev","Pris","L1","L2","Rev2","Pris2","L12","L22"]

print [len(statesInRbt[x][0]) for x in range(len(statesInRbt))]
#print probInds
#print numVarTypes
#print len(logProbs[0][numVarTypes[0]:numVarTypes[1]])
#print len(data)
#print 'max'
#print math.exp(max(logProbs[2]))
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
#pyplot.subplot(rows,columns,1)
#pyplot.subplots_adjust(left=0.05,right=1.0,bottom=0.15,top=0.95,wspace=0.2,hspace=0.4)

if plotLog:
    colorNorm = colors.Normalize(-100,0)
else:
    colorNorm = colors.Normalize(0,1) # YOU NEED THIS SO STUFF DOESN'T EXPLODDDEEE
# without colorNorm, you can't do repeated plotting and get a useful
# color answer becasue it normalize colors by default

#print len(logProbs)
#print len(logProbs_T)
#print len(logProbs_O)
#print len(logProbs[0])
#raise(Exception)

#pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs[0][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)

for j in range(len(statesInRbt)):
    if j==2:
        print statesInRbt[j][0]
        print statesInRbt[j][1]
        print logProbs[0][probInds[j]:probInds[j+1]]
    if plotLog:
        pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs[0][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)
    else:
        pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs[0][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)

# create the list for the legend
legendList=[]
for i in range(len(data)): 
    legendList.append(modelNames[i]+" - "+str("%.3G" % (data[i][0])))
    
pyplot.legend(legendList,bbox_to_anchor=(0., -.425, 1., .5), loc=3, ncol=2, mode="expand", borderaxespad=0.)
pyplot.colorbar()
pyplot.plot(poses[0][0],poses[1][0],'ro',markersize=mSize)
pyplot.title('initial - '+actionString)

outFile = setPath+'step00.png'
pyplot.savefig(outFile,bbox_inches='tight')
if plotSet:
    pyplot.show()

print actionString
print 'Initialization:'
print [d[0] for d in data]

pyplot.clf()

for i in range(numSteps):

    #####################################  after transition #######################################
    pyplot.hold(True)
    colorNorm = colors.Normalize(-100,0)

    for j in range(len(statesInRbt)):
        if plotLog:
            pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs_T[i+1][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)
        else:
            pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs_T[i+1][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)
        #print 'max'+str(i)+str(j)
        #print math.exp(max(logProbs_T[i+1][probInds[j]:probInds[j+1]]))

    
    # create the list for the legend
    legendList=[]
    for k in range(len(data)): 
        legendList.append(modelNames[k]+" - "+str("%.3G" % (data[k][i+1])))
    
    pyplot.legend(legendList,bbox_to_anchor=(0., -.425, 1., .5), loc=3, ncol=2, mode="expand", borderaxespad=0.)
    pyplot.colorbar()
    #print 'step: '+str(i)
    pyplot.plot(poses[0][i],poses[1][i],'g>',markersize=mSize)
    #print 'start: '+str(poses[0][i])+','+str(poses[1][i])
    #pyplot.plot(poses[0][i+1],poses[1][i+1],'rs',markersize=mSize)
    #print 'end: '+str(poses[0][i+1])+','+str(poses[1][i+1])

    if actionType == 0:
        pyplot.plot(actions[0][i],actions[1][i],'b^',markersize=mSize)
        #print 'action: '+str(actions[0][i])+','+str(actions[1][i])
    elif actionType == 1:
        pyplot.plot(actions[0][i]+poses[0][i],actions[1][i]+poses[1][i],'b^',markersize=mSize)
        #print 'action: '+str(actions[0][i])+','+str(actions[1][i])
        #print 'result: '+str(actions[0][i]+poses[0][i])+','+str(actions[1][i]+poses[1][i])



    #pyplot.plot(obs[0][i],obs[1][i],'yo',markersize=mSize)
    #print 'obs: '+str(obs[0][i])+','+str(obs[1][i])
    pyplot.title('Step '+str(i)+' - After Transition')
    #print 'Distribution:'
    #print [d[i+1] for d in data] 

    outFile = setPath+'step'+str(i)+'T.png'
    pyplot.savefig(outFile,bbox_inches='tight')
    
    if plotSet:
        pyplot.show()
        
    pyplot.hold(False)
    pyplot.clf()






    #####################################    observation    #######################################
    pyplot.hold(True)
    colorNorm = colors.Normalize(-5,5)

    for j in range(len(statesInRbt)):
        if plotLog:
            pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs_O[i+1][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)
        else:
            pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs_O[i+1][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)
        #print 'max'+str(i)+str(j)
        #print math.exp(max(logProbs_O[i+1][probInds[j]:probInds[j+1]]))

    
    # create the list for the legend
    legendList=[]
    for k in range(len(data)): 
        legendList.append(modelNames[k]+" - "+str("%.3G" % (data[k][i+1])))
    
    pyplot.legend(legendList,bbox_to_anchor=(0., -.425, 1., .5), loc=3, ncol=2, mode="expand", borderaxespad=0.)
    pyplot.colorbar()
    #print 'step: '+str(i)
    #pyplot.plot(poses[0][i],poses[1][i],'g>',markersize=mSize)
    #print 'start: '+str(poses[0][i])+','+str(poses[1][i])
    pyplot.plot(poses[0][i+1],poses[1][i+1],'rs',markersize=mSize)
    #print 'end: '+str(poses[0][i+1])+','+str(poses[1][i+1])

    '''
    if actionType == 0:
        pyplot.plot(actions[0][i],actions[1][i],'b^',markersize=mSize)
        #print 'action: '+str(actions[0][i])+','+str(actions[1][i])
    elif actionType == 1:
        pyplot.plot(actions[0][i]+poses[0][i],actions[1][i]+poses[1][i],'b^',markersize=mSize)
        #print 'action: '+str(actions[0][i])+','+str(actions[1][i])
        #print 'result: '+str(actions[0][i]+poses[0][i])+','+str(actions[1][i]+poses[1][i])
    '''


    pyplot.plot(obs[0][i],obs[1][i],'y.',markersize=mSize)
    #print 'obs: '+str(obs[0][i])+','+str(obs[1][i])
    pyplot.title('Step '+str(i)+' - Observation')
    #print 'Distribution:'
    #print [d[i+1] for d in data] 

    outFile = setPath+'step'+str(i)+'O.png'
    pyplot.savefig(outFile,bbox_inches='tight')
    
    if plotSet:
        pyplot.show()
        
    pyplot.hold(False)
    pyplot.clf()








    ##################################### full distribution #######################################
    pyplot.hold(True)
    colorNorm = colors.Normalize(-100,0)

    for j in range(len(statesInRbt)):
        if plotLog:
            pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs[i+1][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)
        else:
            pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs[i+1][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)
        #print 'max'+str(i)+str(j)
        #print math.exp(max(logProbs[i+1][probInds[j]:probInds[j+1]]))

    
    # create the list for the legend
    legendList=[]
    for k in range(len(data)): 
        legendList.append(modelNames[k]+" - "+str("%.3G" % (data[k][i+1])))
    
    pyplot.legend(legendList,bbox_to_anchor=(0., -.425, 1., .5), loc=3, ncol=2, mode="expand", borderaxespad=0.)
    pyplot.colorbar()
    print 'step: '+str(i)
    pyplot.plot(poses[0][i],poses[1][i],'g>',markersize=mSize)
    print 'start: '+str(poses[0][i])+','+str(poses[1][i])
    pyplot.plot(poses[0][i+1],poses[1][i+1],'rs',markersize=mSize)
    print 'end: '+str(poses[0][i+1])+','+str(poses[1][i+1])

    if actionType == 0:
        pyplot.plot(actions[0][i],actions[1][i],'b^',markersize=mSize)
        print 'action: '+str(actions[0][i])+','+str(actions[1][i])
    elif actionType == 1:
        pyplot.plot(actions[0][i]+poses[0][i],actions[1][i]+poses[1][i],'b^',markersize=mSize)
        print 'action: '+str(actions[0][i])+','+str(actions[1][i])
        print 'result: '+str(actions[0][i]+poses[0][i])+','+str(actions[1][i]+poses[1][i])



    pyplot.plot(obs[0][i],obs[1][i],'yo',markersize=mSize)
    print 'obs: '+str(obs[0][i])+','+str(obs[1][i])
    pyplot.title('Step '+str(i)+' - Full Distribution')
    print 'Distribution:'
    print [d[i+1] for d in data] 

    outFile = setPath+'step'+str(i)+'.png'
    pyplot.savefig(outFile,bbox_inches='tight')
    
    if plotSet:
        pyplot.show()
        
    pyplot.hold(False)
    pyplot.clf()

