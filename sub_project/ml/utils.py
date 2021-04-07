import sys
import os
import pandas as pd
import json
import tensorflow.keras as keras
import numbers
from typing import List, Dict
import scipy as s
import scipy.io as sio
import numpy as np

# Print iterations progress
def print_progress(iteration, total, prefix='', suffix='', decimals=1, bar_length=100):
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        bar_length  - Optional  : character length of bar (Int)
    """
    str_format = "{0:." + str(decimals) + "f}"
    percents = str_format.format(100 * (iteration / float(total)))
    filled_length = int(round(bar_length * iteration / float(total)))
    bar = '█' * filled_length + '-' * (bar_length - filled_length)

    sys.stdout.write('\r%s |%s| %s%s %s' % (prefix, bar, percents, '%', suffix)),

    if iteration == total:
        sys.stdout.write('\n')
    sys.stdout.flush()


def params_parse(params_str: str):
    """
    Построение словаря из строки имеющий формат:
    <key1>.<value>_<key2>.<value>_<key3>.<value>
    Если существует ключ без значения то это будет ключ
    в словаре со значением None
    """
    params = {}
    s = params_str.split("_")
    model_name = s[0].split(".")
    begin = 0
    if len(model_name) == 1:
        params["name"] = model_name
        begin = 1
    
    for i in range(begin, len(s)):
        key_value = s[i].split(".")
        print(key_value)
        if len(key_value) == 2:
            try:
                params[key_value[0]] = int(key_value[1])
            except:
                params[key_value[0]] = key_value[1]
        elif len(key_value) == 1:
            params[key_value[0]] = None
        else:
            raise RuntimeError("Invalid value [" + key_value + "] of  struct [" + params_str + "]" )
        
    return params

def load_info(path) -> dict:
    """
    Загрузка информации о модели, которая представлена следующей структурой в файловой системе:
    - history.json - история значений метрик во время обучения
    - Набор файлов в с весами модели, которые формировались во время обучения модели, как набор весов,
    дающий наименьшию ошибку. Файл с наибольшим номером содержит те веса, которые дают лучшие результаты
    """
    dirname = os.path.basename(path)
    info = {"history": None, "params": [], "model": params_parse(dirname)}
    for path, dirs, files in os.walk(path):
        print("--", path)
        for file in files:
            if (file != "history.json" and file != "keras.json"):
                info["params"].append(params_parse(file))
            elif file == "history.json":
                print("----", os.path.join(path, file))
                f = open(os.path.join(path, file), "r")
                j = json.load(f)
                info["history"] = pd.DataFrame(j)
                f.close()
    return info

def load_model(path : str) -> dict:
    """Загрузка модели с весами из последней доступной эпохи
    (следовательно с лучшими весами)
    param: path - Путь до папки с данными по модели"""
    file = open(os.path.join(path, "keras.json"), "r")
    jd = file.read()
    model = keras.models.model_from_json(jd)
    file.close()
    
    file = open(os.path.join(path, "history.json"), "r")
    history = pd.DataFrame(json.load(file))
    file.close()

    root, dirs, files = next(os.walk(path))
    max_epoch = -1
    file_name = ""
    for f in files:
        if f != "history.json" and f != "keras.json":
            p = params_parse(f)
            if isinstance(p["epoch"], numbers.Number) and p["name"][0] == "mse" and p["epoch"] >= max_epoch:
                max_epoch = p["epoch"]
                file_name = f
    model.load_weights(os.path.join(path, file_name))
    return {"model": model, "history": history, "epoch": max_epoch}


def model_info_load(path : str) -> pd.DataFrame:
    """Загрузка полной информации о модели"""
    info = load_info(path)
    if info["history"] is None:
        print("Model don't have a history [", path, "]")
        return None
    for key in info["model"]:
        info["model"][key] = [info["model"][key]]
    max_epoch = -1

    for param in info["params"]:
        print(param)
        if isinstance(param["epoch"], numbers.Number) and param["name"][0] == "mse" and param["epoch"] > max_epoch:
            max_epoch = param["epoch"]
    history = info["history"]
    row = history.iloc[[max_epoch-1]].copy()
    row["epoch"] = max_epoch
    
    for key in info["model"]:
        row[key] = info["model"][key]
    row["path"] = path
    return row


def load_info_all_models(models_root : str) -> pd.DataFrame:
    df = pd.DataFrame()
    root, dirs, files = next(os.walk(models_root))
    for dir in dirs:
        df_info = model_info_load(os.path.join(root, dir))
        if df_info is not None:
            df = pd.concat([df, df_info])
    return df.reset_index()




################################################################################
######### Загрузка Matlab файла с результатами моделирования модели ############
################################################################################


def modelMat2Dict(mat) -> List[Dict]:
    """Конвертирование данных загруженных из mat файл в более удобное предстваление"""
    ret = []
    d = mat['data']
    for i in range(d.shape[0]):
        ret.append({'time': d[i][0][0].reshape(-1),
                    'ox_pos': np.around(d[i][0][1].reshape(-1), 3),
                    'ox_speed': np.around(d[i][0][2].reshape(-1), 3),
                    'duty_cycle': np.around(d[i][0][3].reshape(-1),3),
                    'begin_pos': d[i][0][4][0, 0],
                    'end_pos': d[i][0][5][0, 0]})
    return ret


def allMatModelLoad(path: str) -> List[Dict]:
    """Загрузка данных из mat файла (необходима наличие оперделенной структуры)"""
    ret = []
    for root, dirs, files in os.walk(path):
        for file in files:
            data = sio.loadmat(os.path.join(root, file))
            ret = ret + modelMat2Dict(data)
    return ret


def convert2LongRepresent(value: List[Dict]) -> Dict:
    """Конвертирование данных, из короткого представления в длинное"""
    ret = {}
    for el in value:
        for key in el:
            v = ret.get(key, [])
            v.append(el[key])
            ret[key] = v

    for key in ret:
        try:
            ret[key] = np.concatenate(ret[key])
        except:
            pass
    return ret


def findEndControll(v):
    """Поиск поледней точки в данных, выполняющее управление"""
    index = 0;
    for el in v['duty_cycle'][::-1]:
        if el != 0:
            break
        else:
            index = index + 1
    return len(v['duty_cycle']) - index


def cutData(data: List[Dict]) -> List[Dict]:
    """Обрезка данных по критерию, управление завершено.
    Если ШИМ отличается от нуля (поиск ведется с конца) то выполняется управление
    иначе, управление не выполнятся, соответсвенно данные для нейронной сети не несут ни какой информации
    их необходимо выкинуть"""
    for el in data:
        last_index = findEndControll(el)
        print(last_index)
        for key in el:
            if type(el[key]) == np.ndarray:
                el[key] = el[key][0:last_index]




###############################################################################
### Функции для приминении нейронной сети в качестве прямого нейроэмулятора ###
###############################################################################


def neural_foo(m, current_state, duty_cycle_set):
    state = current_state
    history = []
    for i in range(duty_cycle_set.shape[0]):
        print(duty_cycle_set[i])
        state = np.concatenate([state, np.array([duty_cycle_set[i]])])
        history.append(state)
        state = m.predict(state.reshape(1, -1))[0]
    return np.stack(history)
    



# if __name__ == "__main__":
#     import tensorflow as tf 
#     from tensorflow import keras

#     import numpy as np
#     import scipy as s
#     import scipy.io as sio
#     import neural

#     matlab_path = "../../../../"
#     path = "./data"
#     data = sio.loadmat(matlab_path + "simulation_data2.mat")
#     data = modelMat2Dict(data)
#     cutData(data)
#     #calcErrorControl(data)
#     long = convert2LongRepresent(data)

#     ann = keras.models.load_model("./ipython/resnet_forward_neural_emulator_test.h5");
#     neural_foo(ann, np.array([0, 0]), np.array([1, 1 ,1, 1]))
