import cv2
import numpy as np
import time
import serial
import msvcrt

print "opening camera....."
cap = cv2.VideoCapture(1)
counter = 0
##fourcc = cv2.VideoWriter_fourcc(*'DIVX'
##                                )
##out = cv2.VideoWriter('output.avi',fourcc, 20.0, (640,480))

print "opening serial port ......"
arduino=serial.Serial(6,9600,timeout=1)
time.sleep(2)
if(arduino.read()>0) :
    time.sleep(2);
    while(cap.isOpened()):

        # Take each frame
        _, frame = cap.read()
        if counter<2 :
            counter=counter+10;
        else :    
            start_time=time.time()
            
            # Convert BGR to HSV
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            
            # define range of blue color in HSV
            lower_blue = np.array([105,50,50])
            upper_blue = np.array([115,255,255])

            # Threshold the HSV image to get only blue colors
            mask = cv2.inRange(hsv, lower_blue, upper_blue)
            img = np.copy(mask)
            img=cv2.bilateralFilter(img,15,100,100)
        
          
            image ,contours ,hierarchy = cv2.findContours(img,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

            if len(contours)==0 :
                arduino.write((chr)(0))
                arduino.write((chr)(0))
                for i in range (0,2):
                    z=arduino.read()
                    time.sleep(0.001)
                    print "read value %d"%i
                    print"  %s " %z
                continue
            else :
                command=[]
                areas =  [cv2.contourArea(c) for c in contours]
                max_index = np.argmax(areas)
                print(areas[max_index])
                if(areas[max_index]>3000):
                    cnt = contours[max_index]
                else :
                    arduino.write((chr)(0))
                    arduino.write((chr)(0))
                    for i in range (0,2):
                        z=arduino.read()
                        time.sleep(0.001)
                        print "read value %d"%i
                        print"  %s " %z 
                    continue
                
                img=cv2.drawContours(img, contours, max_index, (255,0,0), 1)
                M = cv2.moments(cnt)
                if M['m00'] != 0:
                    new_cx = int(M['m10']/M['m00'])
                    new_cy = int(M['m01']/M['m00'])
        
                
                img=cv2.circle(img,(new_cx,new_cy),5,(255,255,255),2)

                new_c1=(int)(new_cx/2.5)
                new_c2=(int)(new_cy/1.875)

                a=(chr)(new_c1)
                b=(chr)(new_c2)
                
                error_cx=(new_c1*2.5)-320
                error_cy=(new_c2*1.875)-240
                print "error in x %s" %error_cx
                print "error in y %s" %error_cy
                
                command.append(a)
                command.append(b)

                for i in range (0,len(command)):          
                    arduino.write(command[i])
                for i in range (0,2):
                    z=arduino.read()
                    time.sleep(0.001)
                    print "read value %d"%i
                    print"  %s " %z
            
##                cv2.imshow('frame',gray)
##                cv2.imshow('mask',mask)
                cv2.imshow('img', image)
##                out.write(img)

            print "time taken %s" % (time.time()-start_time)

            k = cv2.waitKey(1) & 0xFF
            if k == 27:
                break
        
    
print "Relaesing port and camera ............"
cap.release()
cv2.destroyAllWindows()
