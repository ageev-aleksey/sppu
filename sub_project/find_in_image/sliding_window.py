import cv2
import numpy as np

WINDOW_SIZE = (50, 50)

def sliding_window(image, stepSize, windowSize):
	# slide a window across the image
	for y in range(0, image.shape[0], stepSize):
		for x in range(0, image.shape[1], stepSize):
			# yield the current window
			yield (x, y, image[y:y + windowSize[1], x:x + windowSize[0]])

if __name__ == "__main__":
	# img = cv2.imread("./img_pos/226 -14.jpg")
	# img = cv2.GaussianBlur(img, (7, 7), 0)
	# print(img.shape)
	# red = img[:, :, 2]
	# POINTS = []
	cap = cv2.VideoCapture("/media/nrx/ADATA UFD/Диссертация/data/20210302_161444_004.avi")
	while True:
		_, img = cap.read()
		cv2.imshow("original", img)
		for x, y, win in sliding_window(img, WINDOW_SIZE[0], WINDOW_SIZE):
			red = win[:, :, 2];
			# параметрическое определение нормального распределения
			mu = red.mean()
			sigma = red.std()
			for win_i in range(win.shape[0]):
				for win_j in range(win.shape[1]):
					if red[win_i, win_j] <= (mu + 3*sigma):
						win[win_i, win_j, 0] = 0
						win[win_i, win_j, 1] = 0
						win[win_i, win_j, 2] = 0

		hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
		for i in range(hsv.shape[0]):
			for j in range(hsv.shape[1]):
				if hsv[i, j, 0] > 18:
					hsv[i, j, 2] = 0

		img = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)
		cv2.imshow("img", img)

	#cv2.imwrite("sliding_window_test.bmp", img)
