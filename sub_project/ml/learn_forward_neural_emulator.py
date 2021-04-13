import utils
import numpy as np
import scipy as s
import scipy.io as sio
import neural
import tensorflow as tf
import tensorflow.keras as k

if __name__ == "__main__":
    path = "./ipython/data"
    data = utils.allMatModelLoad(path)
    utils.cutData(data)
    long = utils.convert2LongRepresent(data)

    # формирование обучающей выборки
    dsize = long["time"].shape[0]
    x = []
    y = []
    for i in range(dsize - 1):
        x.append([long['ox_pos'][i], long['ox_speed'][i], long['duty_cycle'][i]])
        y.append([long['ox_pos'][i + 1], long['ox_speed'][i + 1]])
    x = np.stack(x)
    y = np.stack(y)
    ann = neural.ResNet((3,), 2, 64, 2, 3, 'relu')
    ann = k.Model(inputs=ann.inputs, outputs=ann.outputs)
    ann.compile('adam', 'mse', ['mae'])
    h = ann.fit(x, y, batch_size=32, epochs=100)

    print(h)