
#include<math.h>
#define PI 3.14159
int sonar(int pinN, int pinM);
void stop_();

// Ultrasonic sensors

// LEFT
int LSO_echo = 10;
int LSI_trig = 8;

// RIGHT
int RSO_echo = 12;
int RSI_trig = 11;

// FRONT
int FSO_echo = 7;
int FSI_trig = 4;

// Motors

// RIGHT
int LM1 = 9;
int LM2 = 6;

// LEFT
int RM1 = 5;
int RM2 = 3;

// Speed calibration levels
int MAX_L= 250;
int MAX_R= 220;


int e=15;  // calibrated error value
double rad2deg= 180.0/PI; 

// new variables for tracking
float xDest = 500.0, yDest= 0.0;    // coordinates of the destination
float xpos = 0, ypos = 0;           // bot's local coordinates
int DIR = 0;                    // bot's direction wrt to x-axis
double angleDest = 0;              // angle with destination
double dx = 0.0, dy = 0.0;          // difference in position
double slope=0.0;
double omega= 0.16;                  // radial speed (degree per second)
double turnby= 0.0;


float speedOfSound = 310;

//Distance to Target in centimeters
float pingTime1;
int Distance1;
float pingTime2;
int Distance2;
float pingTime3;
int Distance3;

int thresh = 10;                    // Threshold
    
void setup(){
    Serial.begin(9600);
     pinMode(LSO_echo, OUTPUT);
    pinMode(LSI_trig, INPUT);
    pinMode(RSO_echo, OUTPUT);
    pinMode(RSI_trig, INPUT);
    pinMode(FSO_echo, OUTPUT);
    pinMode(FSI_trig, INPUT);

    pinMode(LM1, OUTPUT);
    pinMode(LM2, OUTPUT);
    pinMode(RM1, OUTPUT);
    pinMode(RM2, OUTPUT);
}

void loop(){
    Distance1 = sonar(LSO_echo , LSI_trig);
    Distance2 = sonar(RSO_echo , RSI_trig);
    Distance3 = sonar(FSO_echo , FSI_trig);
    Serial.print(Distance1);
    Serial.print(" , ");
   Serial.print(Distance2);
   Serial.print(" , ");
   Serial.print(Distance3);
   Serial.print(" , ");
    Distance1=abs(Distance1);
    Distance2=abs(Distance2);
    Distance3=abs(Distance3);
   
   
    boolean L = (Distance1 < 20), R = (Distance2 < 20), F = (Distance3 < 15);

    if(F && !L){ //obstacle in front
        
        left(90);
       
    }
    else if(F && L){ // obstacle in front and left
      right(90);
    }
     else if(!F && !L && !R && DIR!=0){ //align
      align();
      DIR=0;
    }
    //else if(xpos>=xDest){ //reaching destination
     // stop_();
      //delay(5000);
    //}
    else{
     forward();
        // forward condition
    }
    Serial.println(" xpos:  ");
    Serial.print(xpos);
    Serial.print("  ypos:  ");
    Serial.print(ypos);
    Serial.println("  ");
   
}

void left(double angle){
    // change direction and update the angle
     
    Serial.print("LEFT   ");
    analogWrite(LM1,0);
    analogWrite(LM2,MAX_L);
    analogWrite(RM1,MAX_R);
    analogWrite(RM2,0);
    int duration= (int) abs(angle/omega);
    Serial.println(duration);
    delay(duration);
    DIR= (int)(DIR+ angle);
//    if(DIR<0){
//      DIR = 360 + DIR;
//    }
    
  
  Serial.print("end");
}

void right(double angle){
    // change direction and update the angle
    
    Serial.print("RIGHT    ");
    int duration= (int) abs(angle/omega);
    analogWrite(LM1,MAX_L);
    analogWrite(LM2,0);
    analogWrite(RM1,0);
    analogWrite(RM2,MAX_R);
   
    Serial.println(duration);
    delay(duration);
    DIR= (DIR- angle);
//    if(DIR<0){
//      DIR = 360 + DIR;
//    }
 
   Serial.print("end");
}

void forward(){
    // move forward and update xpos, ypos
    Serial.println("FORWARD  ");
    analogWrite(LM1,MAX_L);
    analogWrite(LM2,0);
    analogWrite(RM1,MAX_R);
    analogWrite(RM2,0);
    // update pos
    double angle= DIR/rad2deg;
    xpos= xpos+ cos(angle);
    ypos= ypos+ sin(angle);
  
 //   stop_();
}

void align(){
  analogWrite(LM1,MAX_L);
    analogWrite(LM2,0);
    analogWrite(RM1,MAX_R);
    analogWrite(RM2,0);
    delay(1000);
    // align the bot towards the destination, use the left and right function calls
    Serial.println(xpos);
    Serial.println(ypos);
    dy= abs(yDest-ypos);
    dx= xDest-xpos;
    slope= dy/dx;
    angleDest = atan(slope)*rad2deg;
    Serial.println("ALIGN ");
    Serial.print("  DIR_inALign: ");
    Serial.println(DIR);
    Serial.print("  angleDest: ");
    Serial.println(angleDest);
        
    if(DIR>0){
            Serial.print("Loop1:  ");
          
           
            turnby=(DIR + angleDest+e);
            Serial.println(DIR);
            Serial.println(angleDest);
            Serial.println("turnby");
            Serial.println(turnby);
            
            right(turnby);
           
            Serial.println("right_align");
            }
    else if(DIR<0){
          Serial.print("Loop3");
            turnby= ((-DIR) + angleDest+e);
            Serial.println(DIR);
            Serial.println(angleDest);
            Serial.println("turnby");
           Serial.println(turnby);
            
              left(turnby);
              
              Serial.println("left_align");
             
            }
 

}


void stop_(){ //code for stoping the car
    analogWrite(LM1,0);
    analogWrite(LM2,0);
    analogWrite(RM1,0);
    analogWrite(RM2,0);
}

int sonar(int pinN, int pinM){ //us sensor data collection function
    int pingTime,Distance;
    digitalWrite(pinN, LOW);        //Set trigger pin low
    delayMicroseconds(2);         //Let signal settle
    digitalWrite(pinN, HIGH);       //Set trigPin high
    delayMicroseconds(5);          //Delay in high state
    digitalWrite(pinN, LOW);        //ping has now been sent
    pingTime = pulseIn(pinM, HIGH); //pingTime is presented in microceconds
    Distance =(speedOfSound*pingTime/10000)/2;
    return Distance;
}
