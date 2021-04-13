import cv2
import os
import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.python.framework.convert_to_constants import convert_variables_to_constants_v2
os.environ["TF_CPP_MIN_LOG_LEVEL"] = '2'
print('tf version:',tf.__version__)

network = keras.models.load_model("model")

# Convert Keras model to ConcreteFunction
full_model = tf.function(network)
full_model = full_model.get_concrete_function(
tf.TensorSpec(network.inputs[0].shape, network.inputs[0].dtype))

# Get frozen ConcreteFunction
frozen_func = convert_variables_to_constants_v2(full_model)
frozen_func.graph.as_graph_def()

layers = [op.name for op in frozen_func.graph.get_operations()]
print("-" * 50)
print("Frozen model layers: ")
for layer in layers:
    print(layer)

print("-" * 50)
print("Frozen model inputs: ")
print(frozen_func.inputs)
print("Frozen model outputs: ")
print(frozen_func.outputs)

# Save frozen graph from frozen ConcreteFunction to hard drive
tf.io.write_graph(graph_or_graph_def=frozen_func.graph,
		logdir="./frozen_models",
		name="frozen_graph.pb",
		as_text=False)