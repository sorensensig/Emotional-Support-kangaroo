#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

MPU6050 mpu6050(Wire);

const int ledPin = 8;
const int vibPin = 9;
const int numLeds = 8;
const int bendPin = A0;
int bendVal = 0;
const int bendThreshold = 340;

long timer = 0;
float threshold = 1.20;
int count = 0;
int randomColourCount = 0;
bool haveBeenShaken = false;

enum state { SHAKE, THROW, INITIAL};

state curState = SHAKE;

//Initial colours from user test
int colours[10][3] = {
  {240, 248, 255},
  {173, 255, 47},
  {105, 105, 105},
  {255, 105, 180}, 
  {255, 255, 0},
  {0, 0, 128},
  {124, 252, 0},
  {255, 165, 0},
  {255, 0, 0},
  {255, 255, 255}
};

int currentColour[3];

Adafruit_NeoPixel pixels(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  //Setup Neopixel LED
  pixels.begin();
  currentColour[0] = 150;currentColour[1] = 0;currentColour[2] = 0;
  ledOn(150, 0, 0);

  //Setup vibration sensor
  pinMode(vibPin, OUTPUT);

  //Setup flex strip
  pinMode(bendPin, INPUT);

}

void loop() {
  mpu6050.update();
  
  flexValue = analogRead(flexPin);         //Read and save analog value from potentiometer
  if(bendVal < bendThreshold) squeeze();
  
  switch(curState){
    case SHAKE:
      handleShake();
      break;
    case THROW:
      handleThrow();
      break;
  }
}

void handleShake(){
  if(getTotalAcc() > threshold){
    //Shake is detected and counting
    Serial.println("Over threshold"); //needs to be over threshold for several rounds. 
    count++;
    haveBeenShaken = true;
    if (count > 2){
        //starting the vibration  
        vibrate(100);
        delay(100);
        vibrate(100);
    }
  }else if(haveBeenShaken && count > 2){
      //Choose from 10 first random colours
      int colourNr = random(10); //Sets one of the colours as the chosen colour
      for(int i = 0; i < 3; i++) currentColour[i] = colours[colourNr][i];
      ledOn(currentColour[0], currentColour[1], currentColour[2]); 
      //resetting the shake
      haveBeenShaken = false;
      count = 0;
  }else{
    count = 0;
    haveBeenShaken = false;
  }
  delay(333);
}

void handleThrow(){
  if(getTotalAcc() > threshold){
    sendData();
    //Change state to initial
  }
}

void squeeze(){
  Serial.println("Squeeze!");
  switch(curState){
    case SHAKE:
      Serial.println("Changed state");
      vibrate(1000);
      curState = THROW;
      break;
    default:
      break;
  }
}

float getTotalAcc(){
  float x = mpu6050.getAccX();
  float y = mpu6050.getAccY();
  float z = mpu6050.getAccZ();
  return sqrt(x*x + y*y + z*z);
}

void ledOn(int r, int g, int b){
  for(int i=0; i<numLeds; i++){
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  }
}

void ledOff(){
  for(int i=0; i<numLeds; i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }
}

void vibrate(int duration){
    digitalWrite(vibPin, HIGH);
    delay(duration);
    digitalWrite(vibPin, LOW);
}

void sendData(){
  Serial.println("Sending!");
  delay(1000);
  vibrate(1000);
  ledOff();
  curState = INITIAL;
}

void test(){
  if(millis() - timer > 1000){
    
    Serial.println("=======================================================");
    Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
  
    Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
    Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
    Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());
  
    Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
    Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
  
    Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
    Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
    Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());
    
    Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
    Serial.print("\tangleY : ");Serial.print(mpu6050.getAngleY());
    Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
    
    Serial.print("total : ");Serial.println(getTotalAcc());
    Serial.println("=======================================================\n");
    timer = millis();
    
  }
}
