// BLYNK CREDENTIALS
#define BLYNK_TEMPLATE_ID     "TMPL3x-af2Kq3"
#define BLYNK_TEMPLATE_NAME   "TESTING"
#define BLYNK_AUTH_TOKEN      "dQ5ENO6Dp5TNnGp6B_47BOYh6YL2pNlb"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// TEMPERATURE SENSOR 
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 10 on the Arduino
#define ONE_WIRE_BUS 10
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
double tempC;

//TDS SENSOR
#include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A0
GravityTDS gravityTds;
double finalTds;

// PH SENSOR
float calib=22.80;// value is set according to ouput
int analogPh; // to read analog input value of p
int reading=A1;
double finalPh;

//RTC MODULE
#include <ThreeWire.h>
#include <RtcDS1302.h>

ThreeWire myWire(7,6,8); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Vishnu";
char pass[] = "12348765";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200
ESP8266 wifi(&EspSerial);


void setup()
{
  Serial.begin(9600);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);

  // TEMPERATURE SENSOR
  // Start up the library
  sensors.begin();

  //TDS SENSOR
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

  // PH SENSOR
  pinMode(reading,OUTPUT);
}

//FUNCTION TO READ TEMPERATURE VALUE
void tempRead()
{
   // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  tempC = sensors.getTempCByIndex(0);
  Blynk.virtualWrite(V0, tempC);

  {
    Serial.print("Temperature is: ");
    Serial.println(tempC);
    delay(4000);
  }
}

//FUNCTION TO READ TDS VALUE
void tdsRead()
{
  //TDS SENSOR
    //temperature = readTemperature();  //add your temperature sensor and read it
    //gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    finalTds = gravityTds.getTdsValue();  // then get the value
    Serial.print("TDS VALUE= ");
    Serial.print(finalTds);
    Serial.println("ppm");
    delay(3000);
    Blynk.virtualWrite(V1, finalTds);
}

//FUNCTION TO READ PH VALUE
void phRead()
{
analogPh = analogRead(reading);
float phVoltage=analogPh*5.0/1024;
finalPh=-5.70 * phVoltage +calib;
Serial.print("PH value");
Serial.print(finalPh);
Serial.print("ph");
Blynk.virtualWrite(V2, finalPh);
}

// SETTING MODES 
int motorSpeed =0; // Set desired speed of motor
struct Mode {
  int motor_Pin;
  int motor_Speed;
  double required_Ph;
  double required_Tds;
  
};

Mode modes[4] = {
  {6.0, 2.0, 7.0, 300}, //  Mode 1 (motor pin,motor speed,requiredPh, requiredtds)
  {9.0, 2.0, 6.5, 500}, //  Mode 2 
  {10.0,2.0, 7.0, 300}, //  Mode 3
  {11.0,2.0, 6.5, 500}, //  Mode 4 
};

int currentMode = 0; // Stores the currently active mode index

BLYNK_WRITE(V3) {
  currentMode = 0; // Set current  Mode 1 -default mode
  adjustValuesBasedOnMode();
}
BLYNK_WRITE(V4) {
  currentMode = 1; // Set current Mode 2
  adjustValuesBasedOnMode();
}
BLYNK_WRITE(V5) {
  currentMode = 2; // Set current Mode 3
  adjustValuesBasedOnMode(); 
}
BLYNK_WRITE(V6) {
  currentMode = 3; // Set current Mode 4
  adjustValuesBasedOnMode();
}

// Function to run motors and adjust values based on mode selected

bool adjustValuesBasedOnMode() {
 
  if (abs(finalPh - modes[currentMode].required_Ph) > 0.2 || abs(finalTds - modes[currentMode].required_Tds) > 50) {
    // Start or adjust motor speed based on deviation from target values
    analogWrite(modes[currentMode].motor_Pin, modes[currentMode].motor_Speed); // Adjust motor speed if needed
    return false; // Motor needs to run or adjust speed (not at target)
  } else {
    // Stop motor if both pH and TDS are within acceptable range
    analogWrite(modes[currentMode].motor_Pin, 0);
    return true; // Motor stopped (reached target)
  }
}

//FUNCTION FOR FOGGERS TURN ON AND OFF
void printTime(const RtcDateTime& dt) {
  char timestring[10];  // Adjust size based on format

  snprintf_P(timestring, 
             countof(timestring),
             PSTR("%02u:%02u"),
             dt.Hour()*60+dt.Minute()
             );

 signed int foggerStartTime=(timestring);
 signed int foggerStopTime=(timestring)+45;
 signed int coolDownendTime=(foggerStopTime)+15;
 int TEMP_THRESHOLD = 28;
 int tempC;
  
 // Check if fogging cycle needs to be started
if (tempC < TEMP_THRESHOLD & foggerStartTime >= coolDownendTime) {
    // Stop foggers due to high temperature
    pinMode(10,HIGH);
}

else if((foggerStartTime >=foggerStopTime)|| tempC >= TEMP_THRESHOLD) {
     // Stop fogging cycle if conditions are met
     pinMode(10,LOW) ,
     foggerStartTime=0;
     foggerStopTime=0;
     coolDownendTime=0;
      delay(1000);
}
} 

void loop(){ 
  Blynk.run();
  tempRead();
  tdsRead();
  phRead();
  RtcDateTime now = Rtc.GetDateTime();
  printTime(now);
  adjustValuesBasedOnMode();

}