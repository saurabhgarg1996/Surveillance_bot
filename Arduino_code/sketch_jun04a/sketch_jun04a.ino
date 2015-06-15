/********************************
ITSP 2K15 TEAM ID 148
PROJECT SURVEILLANCE BOT 
MICROCONTROLLER CODE
********************************/

#include<VarSpeedServo.h>
VarSpeedServo servo_x,servo_y,servo_gun_x,servo_gun_y,gun_trigger;

long int err_sum_x=0;
long int err_sum_y=0;
int last_err_x=0;
int last_err_y=0;
int diff_err_x;
int diff_err_y;
int error_x;
int error_y;

int a;
int b;
int i= 0;
int flag=1;

    
float angle_x=90.0;
float angle_y=20.0;
float angle_x_g=90;
float angle_y_g=20;

double speed_x=0;
double speed_y=0;

double kp=0.15 ,ki=0.015,kd=15;

int start=0;
int shoot=0;
int control=0;

void setup() {
 Serial1.begin(9600);
 
 servo_x.attach(10);
 servo_y.attach(12);
 
 servo_gun_x.attach(3);
 servo_gun_y.attach(5);
 
 gun_trigger.attach(7);
 
 servo_x.write(90,50,true);
 servo_y.write(20,50,true);
 
 servo_gun_x.write(90,50,true);
 servo_gun_y.write(35,50,true);
 
 gun_trigger.write(170,50,true);
 
 Serial.begin(9600);
 
 pinMode(5,OUTPUT);
 digitalWrite(5,LOW);
 flag=1;
}

void loop() {
  if(Serial1.available()>0)
  {
     start=Serial1.read();
    
     if(start=='a')
     {
     while(flag==1)
     {
       if(Serial.available()>0)
        {  
   
         a=Serial.read();
         delay(10);
         b=Serial.read();
         
         Serial.write(a);
         Serial.write(b);
         
         if( (a*2.5)<640 && (b*1.875)<480 && (a*2.5)>5 && (b*1.875)>4 )
         { 
           i=0;
           error_x=(a*2.5)- 320;
           error_y=(b*1.875)- 240;
           
           err_sum_x+=error_x;
           err_sum_y+=error_y;
           
           diff_err_x=error_x-last_err_x;
           diff_err_y=error_y-last_err_y;
           
           angle_x=-float(err_sum_x*0.035)+90;
           angle_y=-float(err_sum_y*0.035)+20;
           
           speed_x = error_x*kp + err_sum_x*ki + diff_err_x*kd;
           speed_y = error_y*kp + err_sum_y*ki + diff_err_y*kd;
           if(abs(error_x)>60)
           {
             servo_x.write(int(angle_x),abs(speed_x),false);
           }
           if(abs(error_y)>40)
           {
             servo_y.write(int(angle_y),abs(speed_y),false);
           }
           
           delay(200);
           
           last_err_x=error_x;
           last_err_y=error_y;
           
         }
          if(a*2.5<=5 && b*1.875<=4)
         {
           if(angle_x>90)
             servo_x.write(int(angle_x)-5*i,5,false);
           else
             servo_x.write(int(angle_x)+5*i,5,false);
           if(angle_y>20)
             servo_y.write(int(angle_y)-5*i,5,false);
           else
             servo_y.write(int(angle_y)+5*i,5,false);
           delay(200);
           i++;
         } 
         
          }
    
        if(Serial1.available()>0)
        {
          shoot=Serial1.read();
          if(shoot=='s')
      {
      
            servo_gun_x.write(int(angle_x+10),30,false);
            servo_gun_y.write(int(angle_y-5),30,false);
            delay(1000);
            gun_trigger.write(135,50,true);
            gun_trigger.write(170,50,true);
            delay(100);
          
            flag=0;
            break;
      } 
        }
   }

  }
      else if(start=='m')
      {   Serial.println("Received");
        while(1)
      {
      if(Serial1.available())
      {
           control= Serial1.read();
           if(control=='r')
           {
             angle_x+=3;
             angle_x_g+=3;
           }
           else if (control=='l')
           {
             angle_x-=3;
             angle_x_g-=3;
           }
           else if (control=='u')
          {
            angle_y+=3;
            angle_y_g+=3;
          }
           else if(control=='d')
          {
            angle_y-=3;
            angle_y_g-=3;
          }
          
          
            else if(control =='s')
            {
            
              delay(100);
              delay(500);
              gun_trigger.write(135,50,true);
              gun_trigger.write(170,50,true);
              delay(100);
              break;
            }
              servo_x.write(angle_x,20,false);
              servo_y.write(angle_y,20,false);
              servo_gun_x.write(angle_x_g,20,false);
              servo_gun_y.write(angle_y_g,20,false);
         }     
        } 
      }
    }
}
