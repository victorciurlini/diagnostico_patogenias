import numpy as np
import matplotlib
matplotlib.use('TkAgg') #renderiza o gráfico
from matplotlib import pyplot as plt
from matplotlib.image import imread
import pythreshold.utils
import scipy.misc
import cv2
import os 

directory_cercosporiose    = "C:/Users/pc home jan 2020/OneDrive/Documentos/2020_2/EMBARCADOS/BASE/cercosporiose"
cercosporiose_images=[]

all_images = os.listdir(directory_cercosporiose)
print(all_images)
for k in range(0, len(all_images)):
    filename = all_images[k]
    k = filename.find('_resized.jpg')
    if k > -1:
        cercosporiose_images.append(filename)


number_images = len(cercosporiose_images)
for j in range(0,number_images):
    img_rd= cv2.imread(cercosporiose_images[j])
    blur = cv2.blur(img_rd,(7,7))
    blur0= cv2.medianBlur(blur,7)
    blur1= cv2.GaussianBlur(blur0,(7,7),0)
    blur2= cv2.bilateralFilter(blur1,9,75,75)
    hsv = cv2.cvtColor(blur2, cv2.COLOR_BGR2HSV)
    #low_green = np.array([58,99,50])
    #high_green = np.array([63,95,28])
    low_green = np.array([0,35,0])
    high_green = np.array([255,95,255])
    mask = cv2.inRange(hsv, low_green, high_green)
    res = cv2.bitwise_and(img_rd,img_rd, mask= mask)
    canny_res = cv2.Canny(res, 75, 50)
    cv2.imwrite(str(j)+'_cercosporiose_treshold.jpg',res)
    cv2.imwrite(str(j)+'_cercosporiose_treshold_canny.jpg',canny_res)
