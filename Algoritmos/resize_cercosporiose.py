import numpy as np
import tkinter
import matplotlib
# matplotlib.use('TkAgg') #renderiza o gráfico
from matplotlib import pyplot as plt
from matplotlib.image import imread
import cv2
import os 
import re


cwd = os.getcwd()
photos_path = re.sub('Algoritmos', 'Photos', cwd)
database_path = re.sub('Algoritmos', 'database', cwd)
cercosporiose_images=[]

all_images = os.listdir(photos_path)
print(all_images)

for k in range(0, len(all_images)):
    filename = all_images[k]
    k = filename.find('.jpg')
    if k > -1:
        cercosporiose_images.append(f"{photos_path}/{filename}")

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
    cv2.imwrite(f'{database_path}/{str(j)}_cercosporiose_resized.jpg',cercosporiose_images_resized)

