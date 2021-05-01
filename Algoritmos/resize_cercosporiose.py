import numpy as np
import matplotlib
matplotlib.use('TkAgg') #renderiza o gráfico
from matplotlib import pyplot as plt
from matplotlib.image import imread
import cv2
import os 

directory_cercosporiose    = "C:/Users/pc home jan 2020/OneDrive/Documentos/2020_2/EMBARCADOS/BASE/CERCOSPORIOSE"
cercosporiose_images=[]

all_images = os.listdir(directory_cercosporiose)
print(all_images)
for k in range(0, len(all_images)):
    filename = all_images[k]
    k = filename.find('.jpg')
    if k > -1:
        cercosporiose_images.append(filename)

cercosporiose_images_0 = imread(cercosporiose_images[0])
plt.imshow(cercosporiose_images_0)
plt.show()
number_images = len(cercosporiose_images)
print('Number images: '+str(number_images))

cercosporiose_images_resized = []
cercosporiose_images_resized_renamed = []

number_images = len(cercosporiose_images)
for j in range(0,number_images):
    img_rd= cv2.imread(cercosporiose_images[j])
    cercosporiose_images_resized= cv2.resize(img_rd,dsize=(550,400))
    cv2.imwrite(str(j)+'_cercosporiose_resized.jpg',cercosporiose_images_resized)

