from PIL import Image
import numpy as np
import sys
import os
import csv

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

#Useful function
def createFileList(myDir, format='.png'):
    fileList = []
    for root, dirs, files in os.walk(myDir, topdown=False):
        for name in files:
            if name.endswith(format):
                fullName = os.path.join(root, name)
                fileList.append(fullName)
    return fileList

# load the original image
myFileList = createFileList(input_folder, extension)

for file in myFileList:
    print(file)
    img_file = Image.open(file)

    # get original image parameters...
    width, height = img_file.size
    new_size = min(width, height)
    format = img_file.format
    mode = img_file.mode

    # Make image Greyscale
    if mode == 'RGBA' or mode == 'RGB':
        img_grey = img_file.convert('L')
    else:
        img_grey = img_file

    # Make square image.
    img_grey = img_grey.resize((new_size, new_size), Image.ANTIALIAS)
    
    # Save Greyscale values
    # value = np.asarray(img_grey.getdata(), dtype=np.int).reshape((img_grey.size[1], img_grey.size[0]))
    value = np.asarray(img_grey.getdata(), dtype=np.int).reshape((img_grey.size[1], img_grey.size[0]))

    with open(output_folder + file.replace(extension, "").replace(input_folder, "") + ".csv", 'wb') as f:
        writer = csv.writer(f)
        f.write(str(new_size) +'\n')
        for v in value:
            writer.writerow(v)
