#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>
#include <FastLED.h>

const int ledPin = 8;
const int numLeds = 12;
CRGB leds[numLeds];
MPU6050 accMeter;
const int vibPin = 9;

// orientation/motion vars
Quaternion qu;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
int16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer


void setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

  Serial.begin(115200);
  FastLED.addLeds<WS2812, ledPin, GRB>(leds, numLeds);
  accMeter.initialize();
  
  // supply your own gyro offsets here, scaled for min sensitivity
  accMeter.setXGyroOffset(220);
  accMeter.setYGyroOffset(76);
  accMeter.setZGyroOffset(-85);
  accMeter.setZAccelOffset(1788); // 1688 factory default for my test chip
  if(accMeter.dmpInitialize() == 0){
    accMeter.CalibrateAccel(6);
    accMeter.CalibrateGyro(6);
    accMeter.PrintActiveOffsets();
    // turn on the DMP, now that it's ready
    accMeter.setDMPEnabled(true);
  }
  randomSeed(analogRead(0));
  ledOn();
  pinMode(vibPin, OUTPUT);
}

void loop(){
  if(isThrown() == true){
    sendData();
  }
  
  String a;
  while(Serial.available()){
    a += Serial.readString();
  }

  // For debuggin purposes
  if(a == "received"){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void ledOn(){
  int r = random(255);
  int g = random(255);
  int b = random(255);
  
  for(int i = 0; i < numLeds; i++){
    leds[i] = CRGB(0, 255, 0);
  }
  FastLED.show();
} 

void ledOff(){
  for(int i = 0; i < numLeds; i++){
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

bool isThrown(){
  if (accMeter.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    accMeter.dmpGetQuaternion(&qu, fifoBuffer);
    accMeter.dmpGetAccel(&aa, fifoBuffer);
    accMeter.dmpGetGravity(&gravity, &qu);
    accMeter.dmpGetLinearAccel(&aaReal, &aa, &gravity);
    if(abs(aaReal.x) > 12000){
      return true;
    }
    else if(abs(aaReal.y) > 12000){
      return true;
    }
    else if(abs(aaReal.z) > 12000){
      return true;
    }
    
  }
  return false;
}

void sendData(){
  Serial.println("Sending!");
  delay(1000);
  vibrateOn();
  delay(1000);
  vibrateOff();
  ledOff();
}

void vibrateOn(){
  digitalWrite(vibPin, HIGH);
}

void vibrateOff(){
  digitalWrite(vibPin, LOW);
}
