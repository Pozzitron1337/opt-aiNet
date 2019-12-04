from os import system, remove
from ctypes import *
import re

lib = None

class AiNet(object):
    def __init__(self, x1, x2, y1, y2):
        system('g++ -c -fPIC aiNetRealization/functions.cpp -o aiNetRealization/functions.o')
        system('g++ -shared -Wl,-soname,libsfunctions.so -o libsfunctions.so aiNetRealization/functions.o')
        global lib
        lib = cdll.LoadLibrary('./libsfunctions.so')
        lib.AiNet_new.argtypes = [c_double, c_double, c_double, c_double]
        self.ainet = lib.AiNet_new(x1, x2, y1, y2)
    def max(self):
        lib.AiNet_max(self.ainet)
    def min(self):
        lib.AiNet_min(self.ainet)



def getDataSet_for_maximum(iterations):
    elements = 12
    file = open("./maxValues.txt", 'r')
    lines = [line for line in file]
    file.close()
    #remove("./maxValues.txt")
    array = {}
    p=[0]*elements
    for i in range(elements):
        p[i]=[0]*2
    j=0
    for i in iterations:
        pattern = "(-?\d+\.\d+),(-?\d+\.\d+)"
        points = re.findall(pattern, lines[i])
        pattern = "-?\d+\.\d+"
        for point in points:
            coordinates = re.findall(pattern, str(point))
            p[j][0]=(coordinates[0])
            p[j][1]=(coordinates[1])
            j+=1
        array[i]=p
        p = [0] * elements
        for it in range(elements):
            p[it] = [0] * 2
        j = 0
    return array



