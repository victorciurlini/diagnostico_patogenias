import numpy as np
import glob
import time
import cv2
import os
import matplotlib.pyplot as plt
import re
from keras.preprocessing import image
from tensorflow.keras.models import model_from_json
from keras.preprocessing.image import ImageDataGenerator

train_datagen = ImageDataGenerator(rescale = 1./255,
                                   shear_range = 0.2,
                                   zoom_range = 0.2,
                                   horizontal_flip = True)
training_set = train_datagen.flow_from_directory('../training_set',
                                                 target_size = (64, 64),
                                                 batch_size = 32,
                                                 class_mode = 'binary')

cwd = os.getcwd()
modelo_path = re.sub('Algoritmos', 'modelo', cwd)
result_path = re.sub('Algoritmos', 'resultado', cwd)
json_file = open(f"{modelo_path}/trained_model_binario.json", 'r')
loaded_model_json = json_file.read()
json_file.close()
cnn = model_from_json(loaded_model_json)
cnn.load_weights(f"{modelo_path}/trained_model_binario.h5")

list_of_files = glob.glob('../database/*') 
latest_file = max(list_of_files, key=os.path.getctime)
list_of_files =[]
tratamento = []
latest_file_renamed =[]
print(list_of_files)

def tratamento_imagem(latest_file):
    img_rd= cv2.imread(latest_file)
    images_resized= cv2.resize(img_rd,dsize=(550,400))
    cv2.imwrite(str(latest_file),images_resized)
    list_of_files_renamed = glob.glob('../database/*') 
    latest_file_renamed = max(list_of_files_renamed, key=os.path.getctime)
    return latest_file_renamed

tratamento = tratamento_imagem(latest_file) 
test_image = image.load_img(tratamento, target_size = (64, 64))
test_image = image.img_to_array(test_image)
test_image = np.expand_dims(test_image, axis = 0)
result = cnn.predict(test_image)
training_set.class_indices

if result[0][0] == 1:
    prediction = 'SAUDAVEL'
else: 
    prediction = 'DOENTE' 

arquivo = []
arquivo = open(f"{modelo_path}/RESULTADO.txt", 'w')
arquivo.close()
