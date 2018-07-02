from PIL import Image
import numpy as np
import sys
import os
import csv
import scipy.misc


if len(sys.argv) < 4:
    print("Correr python csv_converter.py <carpeta_imagenes> <carpeta_output> <extension_imagenes>.")
    print("Por ejemplo: python csv_converter.py imagenes/ imagenes_convertidas/ .png")
    exit(0)

input_folder = sys.argv[1]
output_folder = sys.argv[2]
extension = sys.argv[3]

print "Extension:", extension
print "Carpeta salida:", output_folder
print "Carpeta entrada:", input_folder

# Useful function
def createFileList(myDir, format='.png'):
    fileList = []
    for root, dirs, files in os.walk(myDir, topdown=False):
        for name in files:
            if name.endswith(format):
                fullName = os.path.join(root, name)
                fileList.append(fullName)
    return fileList

# load the original image
myFileList = createFileList(input_folder, '.csv')

for file in myFileList:

    imagen_numpy = np.genfromtxt(file, delimiter=',')
    imagen = scipy.misc.toimage(imagen_numpy, cmin=0.0)
    print "Imagen:", file
    print "Dimensiones:", imagen.size
    imagen.save(file + '.png')
    print(file)
