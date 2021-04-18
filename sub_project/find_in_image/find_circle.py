import cv2

img = cv2.imread("-50 0 -48 0.jpg")
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

for i in range(hsv.shape[0]):
	for j in range(hsv.shape[1]):
		if not ((hsv[i, j, 0] >= 300 and hsv[i, j, 2] >= 80)):
			hsv[i, j, 1] = 0
			hsv[i, j, 2] = 0

img2 = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)

cv2.imwrite("./converted.jpg", img2)
