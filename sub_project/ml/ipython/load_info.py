import sys
import os
sys.path.append(os.getcwd())
print(sys.path)

import model
import neural
import tensorflow as tf
import tensorflow.keras as keras
import numpy as np
import pandas as pd
import json
import multiprocessing
import utils



df = utils.load_info_all_models("models2")