#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
int s=0;


char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial SIM900(4, 5);
 int timesTosend=1;
int count=0;
int numring=0;
int comring=3; 
int onoff=0; // 0 = off, 1 = on
 
int sense=0;

void setup()
{
  Serial.begin(9600);
 
 lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Smart Dustbin");
  Serial.println("GPS Vehicle Tracking System");
  Serial.println();
 pinMode(6, OUTPUT);
 digitalWrite(6, LOW);
  SIM900.begin(9600);
 
   ss.begin(GPSBaud);
  delay(100); 
  pinMode(A1, INPUT);
 //digitalWrite(A1, HIGH);
   pinMode(A0, INPUT);
   digitalWrite(A0, HIGH);
   pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
   pinMode(11, OUTPUT);
   digitalWrite(11, LOW);
  analogReference(DEFAULT);
}





void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  //if(digitalRead(A0)==0 && s==0)
  //{
    //s = 1;
  //}
  Serial.println(analogRead(A1));
  if(analogRead(A1)>1000)
      {
         Serial.println("trigger");
        sense = 1;
        s = 1;
        digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  delay(1000);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
      }
       if(analogRead(A1) < 1000)
      {
        sense = 0;
        count = 0;
      }
  Serial.println(analogRead(A0));
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {
      displayInfo();
      if(analogRead(A0) < 600)
      {
        sense = 1;
      }
       if(analogRead(A0) > 600)
      {
        sense = 0;
        count = 0;
      }
      
     
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    //while(true);
  }
  
}

void displayInfo()
{
  Serial.print(analogRead(A1));
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {

  if((s==1 ||sense == 1 || analogRead(A1) < 600) && count==0)
  {
    if(s==0)
    {
     digitalWrite(6, HIGH);
    lcd.setCursor(0, 0);
  lcd.print("are you ok?");
  lcd.setCursor(0, 1);
  lcd.print("press reset");
  delay(10000);
   digitalWrite(6, LOW);
  
    }
    lcd.clear();
   lcd.setCursor(0, 0);
  lcd.print("sending SMS");
  
          SIM900.begin(9600);
      
       SIM900.println("AT+CMGF=1"); //set GSM to text mode
  delay(200);
  while(count<timesTosend){
delay(1500);
 SIM900.print("AT+CMGS=\"");
 SIM900.print("8826782180");
 SIM900.println("\"");
while ( SIM900.read()!='>');
{
  
 if(s==0)
 {
   SIM900.println("Emergency:");
 }
 else
 {
  s=0;
  SIM900.println("metal detected location:");
 }
    SIM900.println("Lat -");
 SIM900.println(gps.location.lat());  //SMS body
delay(500);
SIM900.println();
SIM900.println("Lng -");
SIM900.println(gps.location.lng());  //SMS body
delay(500);
 SIM900.write(0x1A);  // sends ctrl+z end of message
     SIM900.write(0x0D);  // Carriage Return in Hex
 SIM900.write(0x0A);  // Line feed in Hex

//The 0D0A pair of characters is the signal for the end of a line and beginning of another.
lcd.clear();


   lcd.setCursor(0, 0);
  lcd.print("SMS Sent");
delay(5000);
 lcd.clear();
 lcd.setCursor(0, 0);
  lcd.print("Smart Dustbin");
}
count++;
} 
  }
 ss.begin(GPSBaud);
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6);
  }
  else
  {
    Serial.println(F("INVALID"));
  }

  


}
