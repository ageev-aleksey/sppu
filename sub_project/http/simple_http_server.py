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
        ann.predict(np.array([ox_pos, ox_speed, ox_end_pos]))[0, 0]
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
        for i in range(duty_cycle_set.shape[0]):
            state = np.concatenate([state, np.array([duty_cycle_set[i]])])
            state = m.predict(state.reshape(1, -1))[0]
        return state

    def neural_aim(x):
        """Функция оценки качества управления управлющим набором х"""
        state = neural_foo(ann, np.array([0, 0]), x)
        return (state[0] - ox_end_pos)**2


    # diff = np.sign(ox_end_pos - ox_pos)
    # de_init = 0
    # if ox_end_pos < ox_pos:
    #     de_init = a
    # else:
    #     de_init = b
    
    res = optimize.differential_evolution(neural_aim, np.array([(-1, 1), (-1, 1), (-1, 1), (-1, 1), (-1, 1)]),
                                maxiter=2, popsize=1, polish = False, seed = 0)
    return round(float(res.x[0]), 1)


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
        #resp = inverse_neuro_controller(req['ox_pos'], req['ox_speed'], req['end_pos']) #controll by state
        resp = forward_neuro_controller(req['ox_pos'], req['ox_speed'], req['end_pos'])
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
    ann = keras.models.load_model('resnet_forward_neural_emulator_test.h5')
    print("...Model loaded")
    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()