import tensorflow as tf
import tensorflow.keras as keras
import numpy as np
import matplotlib.pyplot as plt
import os
#import pydot
from scipy.spatial import distance
from typing import List


class Ann:
    """Базовый класс искуственной нейронной сети"""
    _delimetr = "_"
    def _as_str(self, key, value):
        return key + "{" + str(value) + "}"
    def _start_name(self):
        return "MODEL"
    @property
    def name(self):
        raise NotImplementedError



class ResBlock (keras.layers.Layer):
    def __init__(self, nunits, size, activation, regularization = None):
        """nunits - число нейронов в слое
        size - размер блока
        activation - функция активации"""
        self.__nunits = nunits
        self.__nlayers = size
        self.__activation = activation
        self.__regularization = regularization
    def __call__(self, x):
        inputs = x
        hiden = inputs
        for i in range(self.__nlayers):
            layer = keras.layers.Dense(self.__nunits, self.__activation, kernel_regularizer = self.__regularization)
            hiden = layer(hiden)
        return keras.layers.add([hiden, inputs])

class ResNet (Ann):
    def __init__(self, shape_input, shape_output, nunits, nblocks, size, activation, regularization = None):
        """
        shape_input - размерность входа
        shape_output - размерность выхода
        nunits - число нейронов в скрытом слое
        nblocks - число ResNet блоков
        size - число слоев в ResNet блоке
        activation - функция активации
        regularization - регулиризация
        """
        inputs = keras.layers.Input(shape = shape_input)
        layer = keras.layers.Dense(nunits, activation, kernel_regularizer = regularization)(inputs)
        for i in range(nblocks):
            layer = ResBlock(nunits, size, activation)(layer)
        outputs = keras.layers.Dense(shape_output, "linear")(layer)
        self.__inputs = inputs
        self.__outputs = outputs
        self.__shape_input = shape_input
        self.__shape_output = shape_output
        self.__nunits = nunits
        self.__nblocks = nblocks
        self.__size = size
        self.__activation = activation
    
    @property
    def inputs(self):
        return self.__inputs
    
    @property
    def outputs(self):
        return self.__outputs
    
    @property
    def name(self):
        s = self._start_name()
        s = s + Ann._delimetr + self._as_str("name", "resnet")
        s = s + Ann._delimetr + self._as_str("nlayers", self.__nblocks*self.__size+2)
        s = s + Ann._delimetr + self._as_str("nunits", self.__nunits)
        return s


class RelativeApproximationError(tf.keras.metrics.Metric):
    """RAE = (1/n) * SUM_{0}^{n}{(y_p - y)/y}"""
    def __init__(self, name='RAE', **kwargs):
        super(RelativeApproximationError, self).__init__(name=name, **kwargs)
        self.value = self.add_weight(name='value',
                                     dtype = tf.float32, initializer='zeros')
        self.num = self.add_weight(name='num', 
                                   dtype = tf.float32, initializer='ones')
    
    def update_state(self, y_true, y_pred, sample_weight=None):
            val =tf.math.abs(tf.math.subtract(y_true, y_pred, name="val"))
            self.value.assign_add(tf.math.reduce_sum(tf.math.divide(val, y_true)), name="value")
            self.num.assign_add(1, name="num")
        
    def result(self):
        return self.value / self.num
    
    def reset_states(self):
        self.value = 0
        self.num = 0


class NeuralMaker:
    """Базовый класс для построения неросетевых моделей"""
    def __init__(self):
        self._isNext = True
    def isNext(self):
        return self._isNext
    def next(self) -> Ann:
        raise NotImplementedError

class ResNetPoperties:
    def __init__(self, shape_input, shape_output, nunits, nblocks, size, activation = "relu", regularization = None, path = "", optimizer = "Adam"):
        self.shape_input = shape_input
        self.shape_output = shape_output
        self.nunits = nunits
        self.nblocks = nblocks
        self.size = size
        self.activation = activation
        self.regularization = regularization
        self.path = path
        self.optimizer = optimizer
    
    @staticmethod
    def from_dict(data : dict):
        prop = ResNetPoperties(data["shape_input"], data["shape_output"], data["nunits"], data["nblocks"],
        data["size"], data["activation"], data["regularization"], data["path"], data["optimizer"])
        return prop
    
    def to_dict(self) -> dict:
        return {
            "shape_input": self.shape_input,
            "shape_output": self.shape_output,
            "nunits": self.nunits,
            "nblocks": self.nblocks,
            "size": self.size,
            "activation": self.activation,
            "regularization": self.regularization,
            "path": self.path,
            "optimizer": self.optimizer
        }
    
class ResNetMaker (NeuralMaker):
    """Построитель ИНС типа ResNet"""
    def __init__(self, props : List[ResNetPoperties]):
        self.props = props
        self.model_index = 0;
    
    def next(self, is_print = False):
        self.model_index = 0;
        for p in self.props:
            self.model_index = self.model_index + 1
            if is_print:
                #utils.print_progress(self.model_index, len(self.props))
                print("Model", self.model_index, "from", len(self.props))
            resnet = neural.ResNet(p.shape_input, p.shape_output, p.nunits, p.nblocks, p.size, p.activation, p.regularization, p.optimizer)
            resnet = keras.Model(inputs=resnet.inputs, outputs=resnet.outputs)
            resnet.compile(keras.optimizers.Adam(), loss="mse", metrics=['mae',neural.RelativeApproximationError()])
            yield {"model": resnet, "props": p}
