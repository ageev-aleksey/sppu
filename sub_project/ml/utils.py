import sys
import os
import pandas as pd

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
    s = params_str.split("_")
    model_name = s[0]
    params = {"name": model_name}
    for i in range(1, len(s)):
        key_value = s[i].split(".")
        if len(key_value) == 2:
            params[key_value[0]] = int(key_value[1])
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
    info = {"history": None, "params": [], "model": params_parse(path)}
    for path, dirs, files in os.walk(path):
        print("--", path)
        for file in files:
            if (file != "history.json"):
                info["params"].append(params_parse(file))
            else:
                print("----", os.path.join(path, file))
                f = open(os.path.join(path, file), "r")
                j = json.load(f)
                info["history"] = pd.DataFrame(j)
                f.close()
    return info



def model_info_load(path : str) -> pd.DataFrame:
    """Загрузка полной информации о модели"""
    info = load_info(path)
    for key in info["model"]:
        info["model"][key] = [info["model"][key]]
    max_epoch = -1

    for param in info["params"]:
        if param["epoch"] > max_epoch:
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
    root, dirs, files = next(os.walk(model_root))
    for dir in dirs:
        df = pd.concat([df, model_info_load(os.path.join(root, dir))])
    return df.reset_index()