from multiprocessing import Pool
import os

value = os.getpid()


def worker(v):
    print("Proc pid:", os.getpid())
    print("pid in value:", value)


if __name__ == "__main__":
    p = Pool(2)
    p.map(worker, [0]*10)