import cv2
import numpy as np

WINDOW_SIZE = (30, 30)

def sliding_window(image, stepSize, windowSize):
	# slide a window across the image
	for y in range(0, image.shape[0], stepSize):
		for x in range(0, image.shape[1], stepSize):
			# yield the current window
			yield (x, y, image[y:y + windowSize[1], x:x + windowSize[0]])
centroid = (0, 0)
color = (255, 0, 0)
if __name__ == "__main__":
	# original = cv2.imread("./img_pos/-2 -158.jpg")
	# img = np.copy(original)
	# img = cv2.GaussianBlur(img,(11,11),0)
	# #img = cv2.medianBlur(img, 5)
	# print(img.shape)
	# # red = img[:, :, 2]
	# # POINTS = []
	cap = cv2.VideoCapture("/media/nrx/ADATA UFD/Диссертация/data/20210302_161444_004.avi")
	while True:
		_, original = cap.read()
		#cv2.imshow("original", original)
		img = np.copy(original)
		img = cv2.GaussianBlur(img, (15, 15), 2)
		#img = cv2.medianBlur(img, 7)

		for x, y, win in sliding_window(img, WINDOW_SIZE[0], WINDOW_SIZE):
			red = win[:, :, 2];
		# параметрическое определение нормального распределения
			mu = red.mean()
			sigma = red.std()

			rtable = np.zeros(256, np.uint8)
			identity = np.arange(256, dtype=np.uint8)
			for i in range(int(np.ceil(mu + 3*sigma)), 256):
				rtable[i] = i
			table = np.dstack((identity, identity, rtable))
			cv2.LUT(win, table, win)
	# for win_i in range(win.shape[0]):
	# 	for win_j in range(win.shape[1]):
	# 		if red[win_i, win_j] <= (mu + 3*sigma):
	# 			win[win_i, win_j, 0] = 0
	# 			win[win_i, win_j, 1] = 0
	# 			win[win_i, win_j, 2] = 0

		hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
	# hue_table = np.zeros(256, np.uint8)
	# saturation_table = np.arange(256, dtype=np.uint8)
	# value_table = np.zeros(256, np.uint8)
	# for i in range(18):
	# 	hue_table[i] = i

	# table = np.dstack((hue_table, saturation_table, value_table))
	# for i in range(hsv.shape[0]):
	# 	for j in range(hsv.shape[1]):
	# 		if hsv[i, j, 0] > 18:
	# 			hsv[i, j, 2] = 0
		indx = np.where(hsv[:, :, 0] > 15)
		hsv[indx[0], indx[1], 2] = 0
	# hsv = cv2.LUT(hsv, table)
		bin = hsv[:, :, 2]
		ret, bin = cv2.threshold(bin, 90, 255, cv2.THRESH_BINARY)
		m = cv2.moments(bin, True)
		if m['m00'] != 0:
			centroid = (int(m['m10']/m['m00']), int(m['m01']/m['m00']))
			color = (0, 0, 255)
		else:
			color = (255, 0, 0)
		original = cv2.circle(original, centroid, 10, color, cv2.FILLED)
		cv2.imshow("img", cv2.resize(original, (1280, 720)))
		cv2.imshow("blur", cv2.resize(img, (1280, 720)))
		if cv2.waitKey(1) == 27:
			break
	#cv2.imwrite("sliding_window_test.bmp", original)