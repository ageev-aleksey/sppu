import numpy as np 
import cv
import pandas as pd 

if __name__ == "__main__":
    df = pd.read_csv("C:\\Users\\user\\Documents\\projects\\sppu\\sub_project\\data\\положение\\sppu_info.csv")
    df = df.sort_values(by=["cameraIndex"])
    print(df.head())
