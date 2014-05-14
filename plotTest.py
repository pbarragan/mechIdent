from matplotlib import pyplot as plt
import numpy as np

fig=plt.figure()
plt.plot(np.random.rand(10))


def onclick(event,fig):
    print 'key=%s'%(
        event.key)
    if event.key == 'left':
        print 'yes'
        plt.clf()
        

   

cid = fig.canvas.mpl_connect('key_press_event', onclick)

plt.show()

#fig.canvas.mpl_disconnect(cid)
