#!/usr/bin/python3

import numpy as np # linear algebra
import os

from tensorflow.keras.preprocessing import image
from tensorflow.keras.models import Sequential, load_model

model = load_model("model")
img = image.load_img("../data/fer2013/test/surprise/PrivateTest_139065.jpg",target_size = (48,48),color_mode = "grayscale")
img = np.array(img)
img = np.expand_dims(img,axis = 0) #makes image shape (1,48,48)
img = img.reshape(1,48,48,1)
result = model.predict(img)
result = list(result[0])
label_dict = {0:'Angry',1:'Disgust',2:'Fear',3:'Happy',4:'Neutral',5:'Sad',6:'Surprise'}
img_index = result.index(max(result))
print(label_dict[img_index])