import cv2


img = cv2.imread('neg/neg-0002.jpg')
height, width, channels = img.shape
print (str(width) + ' ' + str(height))