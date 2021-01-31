from typing import List, Callable, Any
from scipy import integrate
import numpy as np
import neural


class LogicError(Exception):
    pass

class DynamicModel:
    """Базововый класс системы ДУ"""
    def __init__(self, name, started_states):
        self.__name = name
        self.__states = started_states
    @property
    def name(self):
        return self.__name
    def setArgs(self, args):
        raise NotImplementedError;
    @property
    def args(self):
        raise NotImplementedError;
    @property
    def started_states(self):
        return self.__states
    def __call__(self, var, t):
        raise NotImplementedError;


## Модель лотки вольтера
class LotkaVoltera (DynamicModel):
    def __init__(self, alpha, beta, delta, gamma, started_states):
        super().__init__("Модель Лотки Волтера", started_states)
        self.setArgs([alpha, beta, delta, gamma])
    def setArgs(self, args):
        if (len(args) != 4):
            raise LogicError
        self.__alpha = args[0]
        self.__beta = args[1]
        self.__delta = args[2]
        self.__gamma = args[3]
    @property
    def args(self):
        return {"alpha": self.__alpha, "beta": self.__beta, 
                "delta": self.__delta, "gamma": self.__gamma} 
    def __call__(self, var, t):
        x, y = var
        dx = self.__alpha*x - self.__beta*x*y
        dy = self.__delta*x*y - self.__gamma*y
        return [dx, dy]

# Модель атрактор Лоренца  
class LorenzSystem (DynamicModel):
    def __init__(self, ro, r, b, started_states):
        super().__init__("Система Лоренца")
        self.setArgs([ro, r, b])
    def setArgs(self, args):
        self.__ro = args[0]
        self.__r = args[1]
        self.__b = args[2]
    
    @property
    def args(self, t):
        return {"ro": self.__ro, "r": self.__r, 
                "b": self.__b} 
    def __call__(self, var, t):
        x, y, z = variabels
        dx = self.__ro*(y-x)
        dy = x*(self.__r-z)-y
        dz = x*y-self.__b*z
        return [dx, dy, dz]



def create_train1step(data : List[float]):
    """
    Разбиение данных на два мессива, чередуюя элементы по очереди,
    то в первый маасив то во второй
    param: data - данные которые необоходимо разбить на два массива
    """
    x = []
    y = []
    for i in range(len(data)-1):
        x.append(data[i])
        y.append(data[i+1])
    return np.stack(x), np.stack(y)


def make_train_of_model(dynamic_model : DynamicModel, t : List[int]):
    """Формирование обучающей выборке на основе динамической модели
        param: dynamic_model Динамическая модель, которая будет использоваться
        для формирования обучающей выборки
        param: t Временной промежуток используемый для решения системы ОДУ
    """
    x = []
    y = []
    for start in dynamic_model.started_states:
        solve = integrate.odeint(dynamic_model, start , t)
        original_space_size = solve.shape[1]
        #solve, rtime = reduce_space(solve, t, space_distance)
        #solve = solve[:, 0:original_space_size]
        val = create_train1step(solve)
        for i in range(len(val[0])):
            x.append(val[0][i])
            y.append(val[1][i])
    return np.stack(x), np.stack(y)


def model_predict(m, start, norm_value,  steps):
    """Приминение модели для получения предстказания траектории"""
    mbegin =np.array([start]) / norm_value;
    predict = [np.array(mbegin)]
    for i in range(steps-1):
        predict.append(m.predict(predict[-1]))
    return np.stack(predict).reshape(-1, 2)

def make_plots(predict_data, model_data):
    """Формирование графиков на основе данных"""
    if len(predict_data) != len(model_data):
        raise RuntimeError("lenthg of predict_data and model_data must be equal")
    time_sequence = [i for i in range(len(predict_data))]
    fig, ax = plt.subplots(2, 3, dpi=500)
    ax[0, 0].plot(time_sequence, predict_data[:, 0], label="predict")
    ax[0, 0].plot(time_sequence, model_data[:, 0], label="solve")
    ax[1, 0].plot(time_sequence, predict_data[:, 1], label="predict")
    ax[1, 0].plot(time_sequence, model_data[:, 1], label="solve")
    ax[0, 1].plot(predict_data[:, 0], predict_data[:, 1])
    ax[1, 1].plot(model_data[:, 0], model_data[:, 1])
    ax[0, 2].plot(time_sequence, predict_data[:, 0] - model_data[:, 0])
    ax[1, 2].plot(time_sequence, predict_data[:, 1] - model_data[:, 1])
    fig.legend()
   
    
def model_predict_test(ann_model : neural.Ann,
                       analitic_model : Callable[[List[Any], List[float]], List[float]],
                       start : List[int],
                       norm_value : float,
                       steps : int,
                       time_end : float):
    predict = model_predict(ann_model, start, norm_value,  steps)
    time_sequence = np.linspace(0, time_end, steps)
    solve = integrate.odeint(analitic_model, start , time_sequence)
    solve = solve/norm_value
    make_plots(predict, solve)