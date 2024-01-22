#include <Servo.h>
#include <DS3231.h>
DS3231 rtc(SDA, SCL);

Time t;

Servo myservo;

#define ldr1 A0 
#define ldr2 A1 

int pos = 90; 
int tolerance = 20;

const int pingPin = 7; 
const int echoPin = 6; 

void setup() {
  rtc.begin();
  Serial.begin(115200);
  
myservo.attach(2); 
pinMode(ldr1, INPUT); 
pinMode(ldr2, INPUT);
pinMode(l2, OUTPUT);
pinMode(4, OUTPUT);
myservo.write(pos);
delay(1000);

}

void loop() {
  
  t=rtc.getTime();

   int h = t.hour;
   int m = t.min;


   if ((h==06)&&(m==30)){
    digitalWrite(4, HIGH);
    delay(150000);
    digitalWrite(4, LOW);
    }

    int val1 = analogRead(ldr1);
int val2 = analogRead(ldr2);
      
if((abs(val1 - val2) <= tolerance) || (abs(val2 - val1) <= tolerance)) {}

else { 
       if(val1 > val2){pos = pos+1;}
       if(val1 < val2){pos = pos-1;}
     }
     
if(pos > 180) {pos = 180;} 
if(pos < 0)   {pos = 0;} 

myservo.write(pos);
delay(50);

  long duration, inches;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = duration/74/2;



   if (inches<40){
    digitalWrite(12, HIGH);
    delay(150);
    digitalWrite(12, LOW);
    }
    else{
      digitalWrite(12, LOW);
      }




  


}
