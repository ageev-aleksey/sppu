from multiprocessing import Pool
import neural
import model
import tensorflow.keras as keras
import numpy as np
import os
import argparse
import json
import utils
import pandas as pd

def learning_rate_update(epoch: int, lr: float) -> float:
    if epoch % 10 == 0:
        return lr*0.5
    return lr

LR_SCHADULER_CALLBACK = keras.callbacks.LearningRateScheduler(learning_rate_update)
X = None
Y = None
X_test = None
Y_test = None
max_train = None
nax_test = None


def make_dynamic_model(states):
    return model.LotkaVoltera(2/3, 4/3, 1, 1, states)

def make_data():
    #Формируем модель и список начальных состояний
    num_states = 50 #число начальных состояний
    t_start = 0
    t_stop = 10
    value = np.around(np.linspace(0.05, 1, num_states), 3)
    state = []
    for i in value:
        for j in value:
            if i > j and i != 0:
                state.append((i, j))
    state_train = state[0::2]
    state_test = state[1::2]

    m_train = make_dynamic_model(state_train)
    m_test = make_dynamic_model(state_test)
    #t = np.arange(t_start, t_stop, 0.1)
    t = np.linspace(t_start, t_stop, 50)

#Формируем обучающую выборку
    (X, Y) = model.make_train_of_model(m_train, t)
    max_train = np.max(X, axis=0);
   # y_max = np.max(Y,  axis=0);
    X = X/max_train
    Y = Y/max_train

    #Формируем тестовую выборку
    (X_test, Y_test) =model.make_train_of_model(m_test, t)
    max_test = np.max(X_test, axis=0)
    X_test = X_test/max_test
    Y_test =  Y_test/max_test

    return (X, Y, X_test, Y_test, max_train, max_test)



def make_model(p : neural.ResNetPoperties) -> neural.ResNet:

     """Построение модели ResNet на основе переданных параметров"""
     resnet = neural.ResNet(p.shape_input, p.shape_output, p.nunits, p.nblocks, p.size, p.activation, p.regularization)
     resnet = keras.Model(inputs=resnet.inputs, outputs=resnet.outputs)
     resnet.compile(keras.optimizers.Adam(), loss="mse", metrics=['mae',neural.RelativeApproximationError()])
     return {"model": resnet, "props": p}


def worker (prop : neural.ResNetPoperties):
    value = make_model(prop)
    ann = value["model"]
    try:
        os.makedirs(path)
    except:
        pass
    saver = keras.callbacks.ModelCheckpoint(
                filepath=prop.path + "/epoch.{epoch}",
                save_weights_only=False,
                monitor='val_loss',
                mode='min',
                save_best_only=True)

    h = ann.fit(X, Y, batch_size=32, epochs=10, verbose=0,
    validation_data = [X_test, Y_test],  callbacks=[saver, LR_SCHADULER_CALLBACK])
    
    j = pd.DataFrame(h.history)
    file = open(os.path.join(prop.path, "history.json"), "w")
    file.write(j.to_json())
    file.close()

def make_model_props():
    #Формирование моделей
    props = []
    for nunits in [4, 8, 16, 32, 64, 128]:
        for nblocks in [1, 2, 3, 4, 5]:
            for blockSize in [2, 3, 4]:
                path = "./models2/resnet_nunits.{nunits}_nblocks.{nblocks}_blockSize.{blocksize}_relu".format(nunits=nunits, nblocks=nblocks, blocksize=blockSize)
                try:
                    os.makedirs(path)
                except:
                    print("Ошибка создания файла")
                    pass
                p = neural.ResNetPoperties(
                    2, 2,
                    nunits,
                    nblocks,
                    blockSize,
                    regularization = "l1",
                    path = path
                )
                props.append(p)
    return props


def parse_arguments() -> dict:
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", type = str, nargs=1, help = "Файл из которого выполнить чтение или в который записать")
    parser.add_argument("--proc", type = int, nargs=2, help = "[число процессов, индекс текущего процесса] - запуск обучения множества моделей")
    return vars(parser.parse_args())

def print_help(parser : argparse.ArgumentParser):
    print("""Пhограмма для обучения множества моделей нейронных сетей.
     Если указана только file, то будет сгенерировано описание моделей и записано в файл.
     Если так же указан --proc, то из указанного файла будет выполнено, чтение, и зупустится обучение часть моделей""")
    parser.print_help()

if __name__ == "__main__":
    # props = make_model_props()
    # print ("Num neural models:", len(props))
    # # pool = Pool(4)
    # # # pool.map(worker, props)
    # for p in props:
    #     worker(p)
    arguments = parse_arguments()
    if arguments["proc"] != None:
        path = arguments["file"][0]
        f = open(path, "r")
        json_props = json.load(f)
        f.close()
        props = []
        for i in range(arguments["proc"][1], len(json_props), arguments["proc"][0]):
            props.append(neural.ResNetPoperties.from_dict(json_props[i]))
        
        X, Y, X_test, Y_test, max_train, max_test = make_data()
        print("Models:", len(props))
        i = 0
        for p in props:
            utils.print_progress(i, len(props))
            worker(p)
            i = i + 1
    else:
        json_res = []
        for p in make_model_props():
            json_res.append(p.to_dict())
        f = open(arguments["file"][0], "w")
        json.dump(json_res, f)
        f.close()


