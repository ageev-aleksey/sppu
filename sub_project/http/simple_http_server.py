#!/usr/bin/env python3
"""
Very simple HTTP server in python for logging requests
Usage::
    ./server.py [<port>]
"""
from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
from tensorflow import keras
import json
import numpy as np
import scipy.optimize as optimize
import time


ann = None


a = np.array([[-1, -1, -1, -1, -1],
 [-0.75, -0.75, -0.75, -0.75, -0.75],
 [-0.5, -0.5, -0.5, -0.5, -0.5],
 [-0.25, -0.25, -0.25, -0.25, -0.25],
 [0, 0, 0, 0, 0]])

b = np.array([[1, 1, 1, 1, 1],
 [0.75, 0.75, 0.75, 0.75, 0.75],
 [0.5, 0.5, 0.5, 0.5, 0.5],
 [0.25, 0.25, 0.25, 0.25, 0.25],
 [0, 0, 0, 0, 0]])



def inverse_neuro_controller(ox_pos, ox_speed, ox_end_pos):
    """Приминение инверсного нейрорегулятора"""
    return round(float(
        ann.predict(np.array([[ox_pos, ox_speed, ox_end_pos]]))[0, 0]
    ), 1)


def forward_neuro_controller(ox_pos, ox_speed, ox_end_pos):
    """Реализация прямого нейроэмулятора в качестве алгоритма управления"""
    
    def neural_foo(m, current_state, duty_cycle_set):
        """Вычисление возможного поведения модели, при заданной последовательности изменения ШИМ
                m  : kers.Model
                    нейронная сеть используемая для вычисления состояний динамической системы
                current_state : np.ndarray
                    начальное состояние динамической системы
                duty_cycle_set : np.ndarray
                    последовательнось скважности ШИМ, которая применяется по порядку для вычисления реакции модели
        """
        state = current_state
        #print(state)
        state[0] = state[0]/0.706
        state[1] = state[1]/5.278
        for i in range(duty_cycle_set.shape[0]):
            if np.abs(current_state[0]) >= 1 and np.sign(current_state[0]) == np.sign(current_state[1]):
                current_state[0] = np.sign(current_state[0])*(1)
                current_state[1] = 0
            state = np.concatenate([state, np.array([duty_cycle_set[i]])])
            state = m.predict(state.reshape(1, -1))[0]
        return state

    def neural_aim(x):
        """Функция оценки качества управления управлющим набором х"""
        state = neural_foo(ann, np.array([ox_pos, ox_speed]), x)
        return (state[0] - ox_end_pos)**2

    def neural_foo2(m, current_state, duty_cycle_set):
        states = [current_state]
        states[0][0] = states[0][0]/0.706
        states[0][1] = states[0][1]/5.278
        current_state = states[0]
        for i in range(duty_cycle_set.shape[0]):
            if np.abs(current_state[0]) >= 1 and np.sign(current_state[0]) == np.sign(current_state[1]):
                current_state[0] = np.sign(current_state[0])*(1)
                current_state[1] = 0
            s = np.concatenate([current_state, np.array([duty_cycle_set[i]])])
            current_state = m.predict(s.reshape(1, -1))[0]
            states.append(current_state)
        return np.array(states)

    def neural_aim2(x):
        states = neural_foo2(ann, np.array([ox_pos, ox_speed]), x)
        error = []
        for i in range(states.shape[0]):
            error.append(((states[i][0]-ox_end_pos)**2)/(i+1))
        s = 0
        #print("Error:", str(error))
        for v in error:
            s = s + v
        #print("Sum:", str(s))
        return s




    # diff = np.sign(ox_end_pos - ox_pos)
    # de_init = 0
    # if ox_end_pos < ox_pos:
    #     de_init = a
    # else:
    #     de_init = b
    
    #res = optimize.differential_evolution(neural_aim, np.array([(-1, 1), (-1, 1), (-1, 1), (-1, 1), (-1, 1)]),
    #                            maxiter=2, popsize=1, polish = False, seed = 0)
    N = 15 #  Горизонт прогноза
    # res = optimize.differential_evolution(neural_aim2, np.array([(-1, 1)]*N),
    #     init=np.array([[-1]*N, [-1] + [0]*(N-1),[1]+[0]*(N-1),[1]*N, [0]*N]),
    #     maxiter=5, popsize=10, polish = False, seed = 0)
    start = time.time()
    res = optimize.differential_evolution(neural_aim2, np.array([(-1, 1)]*N),
         init=np.array([[-1]*N, [-1] + [0]*(N-1),[1]+[0]*(N-1),[1]*N, [0]*N]),
         maxiter=200, popsize=100, polish = False, seed = 0)
    print("time ->", time.time() - start);
    print("----")
    print(res)
    return round(float(res.x[0]), 3)


class S(BaseHTTPRequestHandler):
    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        logging.info("GET request,\nPath: %s\nHeaders:\n%s\n", str(self.path), str(self.headers))
        self._set_response()
        self.wfile.write("GET request for {}".format(self.path).encode('utf-8'))

    def do_POST(self):
        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data = self.rfile.read(content_length) # <--- Gets the data itself
        logging.info("POST request,\nPath: %s\nHeaders:\n%s\n\nBody:\n%s\n",
                str(self.path), str(self.headers), post_data.decode('utf-8'))

        req = json.loads(post_data.decode('utf-8'))
        # resp = ann.predict(np.array([[req['error']]])) # controll by error
        resp = inverse_neuro_controller(req['ox_pos'], req['ox_speed'], req['end_pos']) #controll by state
        #resp = forward_neuro_controller(req['ox_pos'], req['ox_speed'], req['end_pos'])
        self._set_response()
        ret = {
            "duty_cycle": resp
        }
        print(ret)
        self.wfile.write(json.dumps(ret).encode('utf-8'))

def run(server_class=HTTPServer, handler_class=S, port=8000):
    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logging.info('Starting httpd...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')

if __name__ == '__main__':
    from sys import argv
    print("Loading model...")
    #ann = keras.models.load_model('resnet_model_controll_by_state.h5')
    #ann = keras.models.load_model('resnet_forward_neural_emulator_test.h5')
    ann = keras.models.load_model('backword_model.432.h5')
    #ann = keras.models.load_model('resnet_forward_neural_emulator_test1.h5')
    #ann = keras.models.load_model('resnet_forward_neural_emulator_test3.h5')
    print("...Model loaded")
    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()