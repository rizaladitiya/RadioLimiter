/*
  Radio Limiter

  created by Rizal Aditiya
*/
#include <EEPROM.h>

int sensorMin = 0;    // sensor minimum, discovered through experiment
int sensorMax = 1023;  // sensor maximum, discovered through experiment

int inRfMin = 0;    // 0 - 100 watt
int inRfMax = 100;  

int outRfMin = 0;    // 0 - 8000 watt
int outRfMax = 8000;  

int reflectRfMin = 0;    // 0 - 50 watt
int reflectRfMax = 50;  

int suhuMin = 0;    // 0 - 100 watt
int suhuMax = 100;  

int suhuPin = A0;       // input sensor temperature
int inRfPin = A1;      // RF in
int outRfPin = A2;     // RF out Forward
int reflectRfPin = A3; // RF Reflected
int V1Pin = A4;
int V2Pin = A5;
int V3Pin = A6;
int V4Pin = A7;
int alarm1 = 0;
int alarm2 = 0;

const int fanStep1Pin = 2;  // Fan 1
const int fanStep2Pin = 3;  // Fan 2
const int bypassPin = 4;  // bypass
const int alarm1Pin = 5;  // reset
const int alarm2Pin = 6;  // reset
const int resetPin = 7;  // reset

// declare variable
float suhuValue = 0;
float inRfValue = 0;
float outRfValue = 0;
float reflectRfValue = 0;
float V1Value = 0;
float V2Value = 0;
float V3Value = 0;
float V4Value = 0;

float suhuH = 35;
float suhuHH = 40;
float suhuHHH = 45;
float suhuHHHH = 50;
float inRfH = 90;
float inRfHH = 95;
float outRfH = 6000;
float outRfHH = 7000;
float reflectRfH = 40;
float reflectRfHH = 45;

float V1ValueH = 4;
float V1ValueHH = 4.5;
float V2ValueH = 4;
float V2ValueHH = 4.5;
float V3ValueH = 4;
float V3ValueHH = 4.5;
float V4ValueH = 4;
float V4ValueHH = 4.5;





void setup() {
  Serial.begin(9600);
  // declare the ledPin as an OUTPUT:
  pinMode(fanStep1Pin, OUTPUT);
  pinMode(fanStep2Pin, OUTPUT);
  pinMode(bypassPin, OUTPUT);
  pinMode(alarm1Pin, OUTPUT);
  pinMode(alarm2Pin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  bool fanStep1PinValue = 0;
  bool fanStep3PinValue = 0;

  //Khusus By Pass Normaly Open
  bool bypassPinValue = 1;
  
  bool alarm1PinValue = 0;
  bool alarm2PinValue = 0;
  bool resetPinValue = 0;

  float f = 0.00f;   //Variable to store data read from EEPROM.
  int eeAddress = 0; //EEPROM address to start reading from

  //By Pass Relay normaly on
  digitalWrite(bypassPin, HIGH);


}


void loop() {
  // baca suhu:
  suhuValue = bacaSuhu(analogRead(suhuPin));
  // baca RF in:
  inRfValue = bacaRfIn(analogRead(inRfPin));
  // baca RF Out:
  outRfValue = bacaRfOut(analogRead(outRfPin));
  // baca RF reflect:
  reflectRfValue = bacaRfReflect(analogRead(reflectRfPin));
  // baca V1:
  V1Value = bacaVolt(analogRead(V1Pin));
  // baca V2:
  V2Value = bacaVolt(analogRead(V2Pin));
  // baca V3:
  V3Value = bacaVolt(analogRead(V3Pin));
  // baca V4:
  V4Value = bacaVolt(analogRead(V4Pin));

  //start awal check semua kondisi interlock
  
 
  // stop the program for <sensorValue> milliseconds:
  Serial.print("Suhu sekarang : ");
  Serial.print(suhuValue);
  Serial.print(" *C");
  Serial.println();

  /* 0-100 watt*/
  Serial.print("RF in : ");
  Serial.print(inRfValue);
  Serial.print(" watt");
  Serial.println();

  /* 0-8000 watt*/
  Serial.print("RF out : ");
  Serial.print(outRfValue);
  Serial.print(" watt");
  Serial.println();

  /* 0-50 watt*/
  Serial.print("RF Reflect : ");
  Serial.print(reflectRfValue);
  Serial.print(" watt");
  Serial.println();

  //step 1 fan
  if (suhuValue >= suhuH && suhuValue < suhuHH) {
    Serial.println("Step 1 FAN");
    digitalWrite(fanStep1Pin, HIGH);
  } else {
    digitalWrite(fanStep1Pin, LOW);
  }

  if (suhuValue >= suhuHH && suhuValue < suhuHHH){
    Serial.println("Step 2 FAN");
    digitalWrite(fanStep2Pin, HIGH);
  } else {
    digitalWrite(fanStep2Pin, LOW);
  }
  
  if (suhuValue >= suhuHHH && suhuValue < suhuHHHH){
    Serial.println("Alarm On");
    digitalWrite(alarm1Pin, HIGH);
  }

  if (suhuValue > suhuHHHH){
    Serial.println("Suhu Matikan Relay");
    digitalWrite(bypassPin, LOW);
  }
  
  // Check RF In

  if (inRfValue >= inRfH && inRfValue < inRfHH){
    Serial.println("RF In Alarm On");
    digitalWrite(alarm1Pin, HIGH);
  } 

  if (inRfValue >= inRfHH){
    Serial.println("RF In Matikan Relay");
    digitalWrite(bypassPin, LOW);
  }


  // Check RF Out

  if (outRfValue >= outRfH && outRfValue < outRfHH){
    Serial.println("RF Out Alarm On");
    digitalWrite(alarm1Pin, HIGH);
  } 

  if (outRfValue >= outRfHH){
    Serial.println("RF Out Matikan Relay");
    digitalWrite(bypassPin, LOW);
  }

  // Check RF Reflect

  if (reflectRfValue >= reflectRfH && reflectRfValue < reflectRfHH){
    Serial.println("RF Reflect Alarm On");
    digitalWrite(alarm1Pin, HIGH);
  } 

  if (reflectRfValue >= reflectRfHH){
    Serial.println("RF Reflect Matikan Relay");
    digitalWrite(bypassPin, LOW);
  }
  


  Serial.println();
  delay(500);
}

void resetAlarm(){
  digitalWrite(alarm1Pin, LOW);
  digitalWrite(alarm2Pin, LOW);
  digitalWrite(bypassPin, HIGH);
}

float bacaVolt(int value){
    return map(value, sensorMin, sensorMax, 0, 5);
}
float bacaRfIn(float value){
    return map(value, sensorMin, sensorMax, inRfMin, inRfMax);
}
float bacaRfOut(float value){
    return map(value, sensorMin, sensorMax, outRfMin, outRfMax);
}
float bacaRfReflect(float value){
    return map(value, sensorMin, sensorMax, reflectRfMin, reflectRfMax);
}
float bacaSuhu(int value){
    return map(value, sensorMin, sensorMax, suhuMin, suhuMax);
}
