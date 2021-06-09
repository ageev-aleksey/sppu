import cv2


camera = cv2.VideoCapture("http://192.168.1.176:8080/video")
while True:
	ret_val, img = camera.read();
	cv2.imshow('my webcam', img)
	if cv2.waitKey(1) == 27: 
		break  # esc to quit
cv2.destroyAllWindows()