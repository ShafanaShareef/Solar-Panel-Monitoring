#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 2          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22     // DHT sensor type, change to DHT11 if using that sensor
#define RX 2
#define TX 1

String AP = "Internet Wifi Name";       // AP NAME
String PASS = "Internet Wifi Password"; // AP PASSWORD
String API = "YOUR API WRITE KEY";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
  
int currentPin = 1;              
double kilos = 0;
int peakPower = 0;
unsigned long startMillis;
unsigned long endMillis;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
SoftwareSerial esp8266(RX,TX); 
 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

  lcd.begin(16,2);              
  lcd.clear();
  lcd.setCursor(0,0);           
  lcd.print("Arduino");
  lcd.setCursor(0,1);           
  lcd.print("Energy Meter");

  Serial.begin(9600);   // Initialize serial communication
  dht.begin();          // Initialize DHT sensor
  startMillis = millis();
}

void loop() {

int current = 0;
  int maxCurrent = 0;
  int minCurrent = 1000;
  for (int i=0 ; i<=200 ; i++)  
  {
    current = analogRead(A1);    
    if(current >= maxCurrent)
      maxCurrent = current;
    else if(current <= minCurrent)
      minCurrent = current;
  }
  if (maxCurrent <= 517)
  {
    maxCurrent = 516;
  }
  double RMSCurrent = ((maxCurrent - 516)*0.707)/11.8337;    
  int RMSPower = 220*RMSCurrent;    
  if (RMSPower > peakPower)
  {
    peakPower = RMSPower;
  }
  endMillis = millis();
  unsigned long time = endMillis - startMillis;
  kilos = kilos + ((double)RMSPower * ((double)time/60/60/1000000));   
  startMillis = millis();
  delay (2000);
  lcd.clear();
  lcd.setCursor(0,0);           
  lcd.print(RMSCurrent);
  lcd.print("A");
  lcd.setCursor(10,0);
  lcd.print(RMSPower);
  lcd.print("W");
  lcd.setCursor(0,1);
  lcd.print(kilos);
  lcd.print("kWh");
  lcd.setCursor(10,1);
  lcd.print(peakPower);
  lcd.print("W");

  delay(2000);          // Wait for 2 seconds between readings

  float temperature = dht.readTemperature();    // Read temperature data
  float humidity = dht.readHumidity();          // Read humidity data

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");



  
 
 String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(RMSCurrent)+"&"+ field2 +"="+String(RMSPower)+"&"+ field3 +"="+String(kilos)+"&"+ field4 +"="+String(peakPower);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}


void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
