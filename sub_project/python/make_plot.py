# from dataclasses import dataclass

from matplotlib import pyplot as plt
from typing import List, Dict
import json
import sys


# @dataclass
# class SddState:
#     ox : int
#     oy : int
#     task_ox : int
#     task_oy : int
#     pwm_ox : int
#     pwm_oy : int
#     time : int

def convert_row_to_col(row_array: List[dict]) -> Dict:
    """Преобразование массива объектов в набор массивов полей объекта
    Формирования списка полей осуществялется по первому жлемента массива
    :param row_array - (list-like) массив словарей одинакового типа.
     """
    if len(row_array) == 0:
        return {}

    col_array = {}
    for k in row_array[0].keys():
        col_array[k] = []

    for el in row_array:
        for key in el.keys():
            col_array[key].append(el[key])

    return col_array


def print_usage():
    print("Use: script.py <data.json>")

# TODO необходимо добавить аргументы командной строки для построения графиков:
#   --all-plot - построить график по каждому полю от времени
#   --show-keys - вывод на экран списка полей по которым можно строить графики
#   --keys <key> [<key> [...]] - построение графиков только по заданным ключам
#   --time <time1> <time2> - построить график на временном промежутке
#   --phase-space <key1> <key2> - построение фазового простантсва по заданным ключам
#
if __name__ == "__main__":
    if len(sys.argv) == 1:
        print_usage()
        sys.exit(0)

    file = open(sys.argv[1])
    array = convert_row_to_col(json.loads(file.read()))

    fig, ax = plt.subplots(2, 3, figsize=(50, 30))

    ax[0, 0].set_title("Position Ox")
    ax[0, 0].plot(array["time"], array["ox"])

    ax[1, 0].set_title("Position Oy")
    ax[1, 0].plot(array["time"], array["oy"])

    ax[0, 1].set_title("Pwm Ox")
    ax[0, 1].plot(array["time"], array["pwmX"])

    ax[1, 1].set_title("Pwm Oy")
    ax[1, 1].plot(array["time"], array["pwmY"])

    ax[0, 2].set_title("Phase-Space: Oxy")
    ax[0, 2].plot(array["ox"], array["oy"])

    plt.show()


