#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

MPU6050 mpu6050(Wire);

//NeoPixel setup
const int LED_PIN = 14;
const int NUMPIXELS = 8;
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

//Flex sensor setup
const int flexPin = A1; 
int flexValue;
float x, y, z;


void setup() {
  Serial.begin(9600);
  //Accelerometer setup
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  //Setup Neopixel LED
  pixels.begin();
  lightUpAllLights(pixels.Color(150,0,0));
}

void loop() {
  //Accelerometer
  mpu6050.update();
  
  flexValue = analogRead(flexPin);
  Serial.println(flexValue);

  x = mpu6050.getAngleX();
  y = mpu6050.getAngleY();
  z= mpu6050.getAngleZ();
  translateAngleToColour(x, y, z);
  delay(100);
}

//Stolen from Thomas' code
void lightUpAllLights(uint32_t color){
  pixels.clear();
  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
  }
}

void translateAngleToColour(float x, float y, float z){
  int R, G, B;
  int highValue = 150;
  int rgbHighValue = 150;
  R = map(x, 0, highValue, 0, rgbHighValue);
  G = map(y, 0, highValue, 0, rgbHighValue);
  B = map(z, 0, highValue, 0, rgbHighValue);
  lightUpAllLights(pixels.Color(R, G, B));
}
