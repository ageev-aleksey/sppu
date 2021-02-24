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


if __name__ == "__main__":
    #df = utils.load_info_all_models("../opt_models2")
    #print(df)
    model = utils.load_model("../opt_models2/resnet-Adagrad_nunits.4_nblocks.1_blockSize.2_relu")