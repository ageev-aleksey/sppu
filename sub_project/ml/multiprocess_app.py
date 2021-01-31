from multiprocessing import Pool
import neural
import model
import tensorflow.keras as keras
import numpy as np


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
    print("Processing:", ann.name)
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

    h = ann.fit(X, Y, batch_size=32, epochs=10, 
    validation_data = [X_test, Y_test],  callbacks=[saver, LR_SCHADULER_CALLBACK])
    
    j = pd.DataFrame(history.history)
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

if __name__ == "__main__":
    X, Y, X_test, Y_test, max_train, max_test = make_data()
    props = make_model_props()
    print ("Num neural models:", len(props))
    pool = Pool(4)
    pool.map(worker, props)