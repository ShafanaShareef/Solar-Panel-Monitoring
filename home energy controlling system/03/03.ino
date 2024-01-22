#include <DS3231.h>
DS3231 rtc(SDA, SCL);

Time t;


const int gridCurrentPin = A0;
const int btCurrentPin = A1;
const int solaCurrentPin = A2;

const int relayPin_grid = 6;
const int relayPin_mensola = 5;
const int relayPin_consola = 4;
const int relayPin_bety = 3;

int sensitivity = 66;
int offsetVoltage = 2500;

int adcValue01= 0;
double adcVoltage01 = 0;
double currentValue01 = 0;


int adcValue02= 0;
double adcVoltage02 = 0;
double currentValue02 = 0;


int adcValue03= 0;
double adcVoltage03 = 0;
double currentValue03 = 0;


void setup() {

  pinMode(relayPin_grid, OUTPUT);
  pinMode(relayPin_mensola, OUTPUT);
  pinMode(relayPin_consola, OUTPUT);
  pinMode(relayPin_bety, OUTPUT);

  rtc.begin();
  Serial.begin(115200);

  



  
}

void loop() {


   t=rtc.getTime();

   int h = t.hour;
   int m = t.min;

   
  
  adcValue01 = analogRead(gridCurrentPin); // 0-1024
  adcVoltage01 = (adcValue01 / 1024.0) * 5000; 
  currentValue01 = ( (adcVoltage01 - offsetVoltage) / sensitivity );
  int loadvoltage01 = currentValue01 * 12;
  

  adcValue02 = analogRead(btCurrentPin); // 0-1024
  adcVoltage02 = (adcValue02 / 1024.0) * 5000; 
  currentValue02 = ( (adcVoltage2 - offsetVoltage) / sensitivity );
  int loadvoltage02 = currentValue02 * 12;
  

  adcValue03 = analogRead(solaCurrentPin); // 0-1024
  adcVoltage03 = (adcValue03 / 1024.0) * 5000; 
  currentValue03 = ( (adcVoltage03 - offsetVoltage) / sensitivity );
  int loadvoltage03 = currentValue03 * 12;

  if (currentValue03<180){
    usegrid();
    }
    else if(currentValue01<180){
      usesola_btychg();
      }
    else if ((currentValue03<180) && (currentValue01<180) ){
      usebty();
      }
    else if((h==08)&&(m==00)){

      usesola();
      
      }
    else if((h==17)&&(m==00)){

      usegrid();
      
      }


  

 
}

void usegrid(){
  digitalWrite(relayPin_grid, HIGH);
  digitalWrite(relayPin_mensola, LOW);
  digitalWrite(relayPin_consola, LOW);
  digitalWrite(relayPin_bety, LOW);

  }
void usebty(){
  digitalWrite(relayPin_grid, LOW);
  digitalWrite(relayPin_mensola, HIGH);
  digitalWrite(relayPin_consola, LOW);
  digitalWrite(relayPin_bety, HIGH);
  }
void usesola_btychg(){
  digitalWrite(relayPin_grid, LOW);
  digitalWrite(relayPin_mensola, HIGH);
  digitalWrite(relayPin_consola, HIGH);
  digitalWrite(relayPin_bety, HIGH);
  }

void usesola(){
  digitalWrite(relayPin_grid, LOW);
  digitalWrite(relayPin_mensola, HIGH);
  digitalWrite(relayPin_consola, HIGH);
  digitalWrite(relayPin_bety, LOW);
  }






  
