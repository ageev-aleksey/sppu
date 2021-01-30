import tensorflow as tf
import tensorflow.keras as keras
import numpy as np
import matplotlib.pyplot as plt
import os
import pydot
from scipy.spatial import distance


class Ann:
    """Базовый класс искуственной нейронной сети"""
    _delimetr = "_"
    def _as_str(self, key, value):
        return key + "{" + str(value) + "}"
    def _start_name(self):
        return "MODEL"
    @property
    def name(self):
        raise NotImplementedError;



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
        self.__inputs = inputs;
        self.__outputs = outputs;
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
        self.value = 0;
        self.num = 0;

