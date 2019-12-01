from aiNet import *
import pylab
from mpl_toolkits.mplot3d import Axes3D
import numpy

def function(x,y):
    return x**2+y**2-10*(numpy.cos(2*numpy.pi*x)+numpy.cos(2*numpy.pi*y))

def getAllPoints (a,b):
    x = numpy.arange(-a, a, 0.001)
    y = numpy.arange(-b, b, 0.1)
    xgrid, ygrid = numpy.meshgrid(x, y)
    zgrid = function(xgrid, ygrid)
    return xgrid, ygrid, zgrid

def draw(a,b,iterations):
    #iterations=[0,100,200,300,499]
    maximums=getDataSet_for_maximum(iterations)
    fig = pylab.figure()
    axes = Axes3D(fig)
    x, y, z = getAllPoints(a, b)
    axes.plot_surface(x, y, z, cmap='binary')

    xdata=[]
    ydata=[]
    zdata=[]
    for i in iterations:
        for j in range(len(maximums[0])):
            xdata.append(float(maximums[i][j][0]))
            ydata.append(float(maximums[i][j][1]))
            zdata.append(function(float(maximums[i][j][0]), float(maximums[i][j][1])))
        axes.scatter(xdata, ydata, zdata, c=zdata, cmap='Accent', linewidth=3)
        xdata=[]
        ydata=[]
        zdata=[]
    pylab.show()


def logic():
    a = 5.15    #окіл значення для х
    b = 5.15    #окіл значення для y
    ainet=AiNet(-a, a, -b, b)  #(x з -a до a) ТА (y з -b до b)
    ainet.max() #запуск алгоритму пошушу максимумів
    iterations = [0, 100, 200, 300, 499]    #Ітерації, на яких береться точки для зображення на малюнку
                                            #За рекомендацією ainet проходить 500 ітерацій.
                                            #У нашому доданку ітерація починається з нуля
    draw(a, b, iterations)  #функція,яка малює тривимірну функцію та маркує точки на цьому графіку,
                            # які знайшов алгоритм ainet
logic()