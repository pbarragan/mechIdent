from matplotlib import pyplot
from matplotlib import colors
import numpy
import math
import os

import readData

# interesting ones from most recent data before 4/15/14
#fileName = 'dataBigBigActions/model9/entropy/data9_4.txt'
#fileName = 'dataBigBoth/model5/entropy/data5_7.txt'
#fileName = 'dataBigBoth/model5/random/data5_5.txt'
#fileName = 'dataBigSmallVar/model4/random/data4_2.txt'
#fileName = 'dataBigMid/model8/random/data8_4.txt'
#fileName = 'dataBigMid/model9/entropy/data9_7.txt'

# real robot
#fileName = 'data/data2Thu_May__1_14_00_36_2014.txt'
#fileName = 'data/data3Thu_May__1_13_27_30_2014.txt'
#fileName = 'data/data2Thu_May__1_13_59_04_2014.txt' # - no error
#fileName = 'data/data3Thu_May__1_13_24_36_2014.txt'
#fileName = 'data/data2Thu_May__1_13_56_43_2014.txt' # - incomplete file


# 5/8/14
#fileName = 'data/data2Thu_May__8_13_18_08_2014.txt'
#fileName = 'data/data2Thu_May__8_13_21_34_2014.txt'
#fileName = 'data/data2Thu_May__8_13_24_02_2014.txt' # no error
#fileName = 'data/data3Thu_May__8_13_31_46_2014.txt'
#fileName = 'data/data3Thu_May__8_13_34_10_2014.txt'
#fileName = 'data/data3Thu_May__8_13_37_26_2014.txt'
#fileName = 'data/data3Thu_May__8_13_40_32_2014.txt'

# 5/13/14
# cT .01 cO .1
#fileName = 'data/data3Tue_May_13_12_11_59_2014.txt' # right on 3
#fileName = 'data/data7Tue_May_13_12_17_57_2014.txt' # right on 7
#fileName = 'data/data3Tue_May_13_12_22_39_2014.txt' # way past 3
#fileName = 'data/data7Tue_May_13_12_26_47_2014.txt' # 90 away from 3 past 7
# cT .04 cO .01
#fileName = 'data/data3Tue_May_13_12_35_18_2014.txt' # past 3
#fileName = 'data/data3Tue_May_13_12_37_17_2014.txt' # past 3 again
#fileName = 'data/data7Tue_May_13_12_40_12_2014.txt' # right on 7 mistake
#fileName = 'data/data7Tue_May_13_12_41_10_2014.txt' # right on 7
#fileName = 'data/data7Tue_May_13_12_43_52_2014.txt' # right on 7

# 5/14/14 back to simulation
#fileName = 'data/data4Wed_May_14_12_10_47_2014.txt'

# 5/20/14
#fileName = 'data/data3Tue_May_20_00_20_57_2014.txt' # huge error
#fileName = 'data/data7Tue_May_20_00_35_42_2014.txt' # correct
#fileName = 'data/data3Tue_May_20_01_01_31_2014.txt' # correct
#fileName = 'data/data3Tue_May_20_00_59_13_2014.txt' # correct
#fileName = 'data/data3Tue_May_20_00_56_20_2014.txt' # error

#5/25/14
#fileName = 'data/data9Sun_May_25_10_25_56_2014.txt' # error
#fileName = 'data/data9Sun_May_25_10_26_56_2014.txt' # error
#fileName = 'data/data5Sun_May_25_10_27_39_2014.txt' # error
fileName = 'data/data5Sun_May_25_10_27_57_2014.txt' # error


folderName = fileName[:fileName.find('.txt')].replace('/','-')+'-ERROR'
setPath = 'dump/'+folderName+'/'

if not os.path.exists(setPath):
    os.mkdir(setPath)

# parameters
plotLog = True
plotSet = False

data, numSteps, numMPPairs, model, statesInRbt, logProbs_T, logProbs_O, logProbs, poses, actions, obs, actionType, numVarTypes = readData.get_data(fileName)

#check if this is an error
lastValues = []
for i in range(numMPPairs):
    lastValues.append(data[i][-1])

bestModel = lastValues.index(max(lastValues))
if model != bestModel:
    print "we have an error"
else:
    model = int(raw_input("There is no error. Enter the model-param index to plot against."))
    #raise Exception('This does not have an error')

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
    colorNorm = colors.Normalize(-10,0)
else:
    colorNorm = colors.Normalize(0,1) # YOU NEED THIS SO STUFF DOESN'T EXPLODDDEEE
# without colorNorm, you can't do repeated plotting and get a useful
# color answer becasue it normalize colors by default


for j in range(len(statesInRbt)):
    if j == model or j == bestModel:
        if j==2:
            print statesInRbt[j][0]
            print statesInRbt[j][1]
            print logProbs[0][probInds[j]:probInds[j+1]]
        if plotLog:
            pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs[0][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)
        else:
            pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs[0][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)
#pyplot.show()
#raise(Exception('yup'))
# create the list for the legend
legendList=[]
for i in range(len(data)):
    if i == model or i == bestModel:
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
        if j == model or j == bestModel:
            if plotLog:
                pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs_T[i+1][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)
                print "Transition:"
                print "model: "+str(j)
                print logProbs_T[i+1][probInds[j]:probInds[j+1]]
            else:
                pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs_T[i+1][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)
        #print 'max'+str(i)+str(j)
        #print math.exp(max(logProbs[i+1][probInds[j]:probInds[j+1]]))

    
    # create the list for the legend
    legendList=[]
    for k in range(len(data)):
        if k == model or k == bestModel:
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



    #pyplot.plot(obs[0][i],obs[1][i],'y.',markersize=mSize)
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
        if j == model or j == bestModel:
            if plotLog:
                pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs_O[i+1][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)
                print "Observation:"
                print "model: "+str(j)
                print logProbs_O[i+1][probInds[j]:probInds[j+1]]
            
            else:
                pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs_O[i+1][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)
        #print 'max'+str(i)+str(j)
        #print math.exp(max(logProbs[i+1][probInds[j]:probInds[j+1]]))

    
    # create the list for the legend
    legendList=[]
    for k in range(len(data)):
        if k == model or k == bestModel:
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
        print 'action: '+str(actions[0][i])+','+str(actions[1][i])
    elif actionType == 1:
        pyplot.plot(actions[0][i]+poses[0][i],actions[1][i]+poses[1][i],'b^',markersize=mSize)
        print 'action: '+str(actions[0][i])+','+str(actions[1][i])
        print 'result: '+str(actions[0][i]+poses[0][i])+','+str(actions[1][i]+poses[1][i])
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
        if j == model or j == bestModel:
            if plotLog:
                pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=logProbs[i+1][probInds[j]:probInds[j+1]],s=sSize,marker=symList[j],norm=colorNorm)
                print "Full:"
                print "model: "+str(j)
                print logProbs[i+1][probInds[j]:probInds[j+1]]
            else:
                pyplot.scatter(statesInRbt[j][0],statesInRbt[j][1],c=[math.exp(x) for x in logProbs[i+1][probInds[j]:probInds[j+1]]],s=sSize,marker=symList[j],norm=colorNorm)
        #print 'max'+str(i)+str(j)
        #print math.exp(max(logProbs[i+1][probInds[j]:probInds[j+1]]))

    
    # create the list for the legend
    legendList=[]
    for k in range(len(data)):
        if k == model or k == bestModel:
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



    pyplot.plot(obs[0][i],obs[1][i],'y.',markersize=mSize)
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