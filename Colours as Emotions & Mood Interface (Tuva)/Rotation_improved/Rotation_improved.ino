#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

MPU6050 mpu6050(Wire);

//NeoPixel setup
const int LED_PIN = 14;
const int NUMPIXELS = 8;
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

//Flex sensor setup
const int bendPin = A1; 
int bendVal;
int bendThreshold = 3000;
float x, y, z;
bool colorSelected = false;


//Vibrationsetup - NEW
int vibrationPin = 33;

void setup() {
  Serial.begin(9600);
  //Accelerometer setup
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  //Setup Neopixel LED
  pixels.begin();
  lightUpAllLights(pixels.Color(150,0,0), 0);

  //Vibration setup
  pinMode(vibrationPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int R, G, B;
  while(!colorSelected){
    mpu6050.update();
    float x, y, z;
  
    x = limitValue(mpu6050.getAngleX());
    y = limitValue(mpu6050.getAngleY());
    z= limitValue(mpu6050.getAngleZ());

    int lowValue = 0;
    int highValue = 360;
    int rgbHighValue = 200;
    //restrict the value to be withoin + and 360
    R = map(abs(x), lowValue, highValue, 0, rgbHighValue);
    G = map(abs(y), lowValue, highValue, 0, rgbHighValue);
    B = map(abs(z), lowValue, highValue, 0, rgbHighValue);
    //When the z values are lower than -360 (often), they just end up being 360
    //all the time. How to account for this? 
    Serial.print("X: "); Serial.print(x); Serial.print(" R: ");Serial.print(R);
    Serial.print(" Y: "); Serial.print(y); Serial.print(" G: ");Serial.print(G);
    Serial.print(" Z: "); Serial.print(z); Serial.print(" B: ");Serial.println(B);  
    lightUpAllLights(pixels.Color(R, G, B), 0);
    delay(100);

    bendVal = analogRead(bendPin);
    //Serial.print(" Bend: "); Serial.print(bendVal); 
    if(bendVal > bendThreshold){
      colorSelected = true;
    }
  }
  if (colorSelected){
    Serial.println("Ferdig!");
    vibrate(); 
    int color[] = {R, G, B}; //NOt using this?
    adjustColor(R, G, B);
    Serial.println(R);
    Serial.println(G);
    Serial.println(B);
  }
  Serial.println("Done!");
  delay(10000000);
  
  //sendMessage(color);
  
}

void vibrate (){
  //Not working
  Serial.println("Vibrating");
  digitalWrite(vibrationPin, HIGH);
  delay(400);
  digitalWrite(vibrationPin, LOW); 
}

void adjustColor (int & R, int & G, int & B){
  int newR, newG, newB;
  bool colorAdjusted = false;

  while(!colorAdjusted){
    mpu6050.update();
    Serial.println("Adjust color mode");
    x = limitValue(mpu6050.getAngleX());
    y = limitValue(mpu6050.getAngleY());
    z= limitValue(mpu6050.getAngleZ());
    Serial.print("X: "); Serial.print(x);
    Serial.print(" Y: "); Serial.print(y); 
    Serial.print(" Z: "); Serial.print(z);
    
  
    int totalMovement = ((x + y + z) / 3)* 10; //Gets the average movement
    int totalValue = map(totalMovement, -360, 360, 1, 19);
    float precentage = totalValue / 10.0;
    Serial.print("PRECENTAGE: "); Serial.println(precentage);
    newR = limitRGBvalue(R*precentage);
    newG = limitRGBvalue(G*precentage);
    newB = limitRGBvalue(B*precentage);
    Serial.print("TotalMovement: "); Serial.println(totalMovement);
    Serial.print("TotalValue: "); Serial.println(totalValue);
    Serial.print("R: "); Serial.print(newR);
    Serial.print(" G: "); Serial.print(newG);
    Serial.print(" B: "); Serial.print(newB);
    lightUpAllLights(pixels.Color(newR, newG, newB), 0);
    delay(100);

    bendVal = analogRead(bendPin);
    if(bendVal > bendThreshold){
      colorAdjusted = true;
    }
  }
  if (colorAdjusted){
    vibrate(); 
    R = newR;
    G = newG;
    B = newB;
  }
}

int limitRGBvalue(int value){
  Serial.print("VALUE: "); Serial.println(value);
  return constrain(value, 10, 255); //Restrcit from not going to 0
}

int limitValue (int value){
  int valueRestriction = 360;
  if(value > 360){
    return limitValue(value-720);
  }
  else if (value < -360){
    return limitValue(value + 720);
  }
  else {
    return value;
  }
}



//Stolen from Thomas' code
void lightUpAllLights(uint32_t color, int wait){
  pixels.clear();
  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
