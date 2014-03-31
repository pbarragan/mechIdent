import subprocess
import os

#model = 0
models = [0,1,2,3,4,5,6,7,8,9]
trials = 10
writeFile = 1
#actionSelectionType = 2
actionSelectionTypes = [1,2]

numExperiments = 10

asTypes = ['simple','random','entropy']

for mNum in models:
    model = mNum

    for asType in actionSelectionTypes:
        actionSelectionType = asType

        path = 'dataBig/model'+str(model)+'/'+asTypes[actionSelectionType]+'/'

        for i in range(numExperiments):
            print "experiment "+str(i)
            print "model "+str(model)
            print "action selection type "+str(actionSelectionType)
            
            command1 = './testRealWorld '+str(model)+' '+str(trials)+' '+str(writeFile)+' '+str(actionSelectionType)
            command2 = 'FILE=$(ls '+path+' -Art | tail -n 1)'
            command4 = 'echo $FILE'
            command3 = 'cp $FILE '+path+'data'+str(model)+'_'+str(i)+'.txt'
            #print command1
            #print command2
            #print command4
            # run commands
            p = subprocess.Popen(command1,shell=True)
            p.wait()

            filelist = os.listdir('data/')
            for j, filename in enumerate(filelist):
                filelist[j] = 'data/'+filename
            #print filelist
            filelist = filter(lambda x: not os.path.isdir(x), filelist)
            newest = max(filelist, key=lambda x: os.stat(x).st_mtime)

            command5 = 'cp '+newest+' '+path+'data'+str(model)+'_'+str(i)+'.txt'
            #print newest

            p = subprocess.Popen(command5,shell=True) 
            p.wait()
