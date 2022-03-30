import numpy as np
import cv2 as c
import _thread
black = (0,0,0)
r = (0,0,255) 
t = 3 
lower_blue = np.array([0,0,0])
upper_blue = np.array([179,255,255])
font = c.FONT_HERSHEY_SIMPLEX
draw = c.LINE_AA
            
kernel = np.ones((5,5), np.uint8)
l = [0,0,0,1,1,1]
def nothing(x):
    l[0] = c.getTrackbarPos("L - H", "Trackbars")
    l[1] = c.getTrackbarPos("L - S", "Trackbars")
    l[2] = c.getTrackbarPos("L - V", "Trackbars")
    l[3] = c.getTrackbarPos("U - H", "Trackbars")
    l[4] = c.getTrackbarPos("U - S", "Trackbars")
    l[5] = c.getTrackbarPos("U - V", "Trackbars")
    pass

c.namedWindow("Trackbars")
c.createTrackbar("L - H", "Trackbars", 0, 179, nothing)
c.createTrackbar("L - S", "Trackbars", 0, 255, nothing)
c.createTrackbar("L - V", "Trackbars", 0, 255, nothing)
c.createTrackbar("U - H", "Trackbars", 1, 179, nothing)
c.createTrackbar("U - S", "Trackbars", 1, 255, nothing)
c.createTrackbar("U - V", "Trackbars", 1, 255, nothing)

#camera = c.VideoCapture(0)
while True:
    #_,f = camera.read() # Use for camera
    f = c.imread("mavi_top.jpg", c.IMREAD_COLOR)
    f = c.resize(f,(300,300))
    blurred_frame = c.GaussianBlur(f,(3,3),0)
    hsv = c.cvtColor(blurred_frame,c.COLOR_BGR2HSV)
    lower_blue = np.array([l[0],l[1],l[2]])
    upper_blue = np.array([l[3],l[4],l[5]])
    mask = c.inRange(hsv,lower_blue,upper_blue)
    mask = c.dilate(mask, kernel, iterations=3) 
    contours, hierarchy = c.findContours(mask, c.RETR_TREE,c.CHAIN_APPROX_NONE)
    
    for contour in contours:
        area = c.contourArea(contour)    
        c.drawContours(f,contours,-1, r, t)
        if area > 1:
            M = c.moments(contour)
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            c.drawContours(f,contours,-1, r, t)
            #print("Coordinates: ",str(cx),"-",str(cy)) # Find Coordinates for tracking object
            c.circle(f, (cx,cy), 5, r, 10)
            c.putText(f,("x:"+str(cx)+"-y:"+str(cy)),(50,50),font,1,black,t,draw)
    
    c.imshow("Frame",f)
    c.imshow("Mask",mask)
    if c.waitKey(100) > 0:
        break

cv.release()
c.destroyAllWindows()









