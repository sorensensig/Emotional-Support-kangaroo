#include <Adafruit_NeoPixel.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

// FILL IN YOUR NAME HERE
string user = "Sigurd";
// FILL IN YOUR NAME HERE

// EVERYONE ELSE'S SETTINGS
//NeoPixel Setup
//#define PIXEL_PIN 6

// TUVA'S PIXEL SETTINGS
#define PIXEL_PIN 14 //Tuva's Neopixel Pin
#define PIXEL_COUNT 8  // Number of NeoPixels



// COMMON SETTINGS
Adafruit_NeoPixel pixels(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//Setup accelerometer with MPU library
MPU6050 mpu6050(Wire);

//int hapticPin = 2;
int hapticPin = 33; //Value for Tuva
int bendPin = A1;

bool vibrating = false;
bool audioPlayed = false;
bool lightsOn = false;
bool messageReceived = false;
bool isRecording = false;

int bendVal = 0;
int bendThreshold;

float dropThreshold = 0.30;
float throwThreshold = 2.70;

int savingCounter = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int tempRed = 0;
int tempGreen = 0;
int tempBlue = 0;
int arr[] = {redValue, greenValue, blueValue};

void setup() {
  switch(user) {
    case "Sigurd":
      bendThreshold = 420;
      break;
    case "Tuva":
      bendThreshold = 3000;
      throwThreshold = 2.00;
      break;
    case "Marie":
      bendThreshold = 340;
      break;
    case "Thomas":
      bendThreshold = 80;
      break;
  }

  if(user == "Tuva") {
    Serial.begin(9600); //Value working for Tuva 
  } else {
    Serial.begin(115200);
  }

  //Vibration and flex sensor setup
  pinMode(hapticPin, OUTPUT);
  pinMode(bendPin, INPUT);

  //NeoPixel setup
  pixels.begin();
  lightUpAllLights(pixels.Color(0, 0, 0), 0);
  
  //Accelerometer setup
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}
void loop() {
  readIncomingMessage();

  bendVal = analogRead(bendPin);

  if(user == "Tuva") {
    // Working for Tuva
    if(messageReceived && bendVal > bendThreshold) {
      listenToMessage();
    } else {
      if(bendVal > bendThreshold){
        isRecording = true;
        Record();
      }
    }
  } else {
   if(messageReceived && bendVal < bendThreshold) {
     listenToMessage();
   } else {
     if(bendVal < bendThreshold){
       isRecording = true;
       Record();
     }
   }
  }
  
  if(!(arr[0] == 0 && arr[1] == 0 && arr[2] == 0) && !audioPlayed) {
    pulsateLights();
  }
}

void Record(){
  lightsOn = false;
  int recordingCounter = 0;
  int minimumRecordTime = 3;
  
  while(isRecording) {
    bendVal = analogRead(bendPin); //Read and save analog value from bend sensor
    digitalWrite(hapticPin, HIGH);
    delay(60);
    digitalWrite(hapticPin, LOW);

    if(!lightsOn){
      lightUpAllLights(pixels.Color(255, 0, 0), 100); 
    } else {
      lightUpAllLights(pixels.Color(0, 0, 0), 50);  
    }

    lightsOn = !lightsOn;   
    delay(500);

    if(user == "Tuva") {
      //Working for Tuva
      if(recordingCounter > minimumRecordTime && bendVal > bendThreshold){
        isRecording = false;
      }
    } else {
      if(recordingCounter > minimumRecordTime && bendVal < bendThreshold){
        isRecording = false;
      }
    }

    recordingCounter++;
  }
  lightUpAllLights(pixels.Color(0, 0, 0), 0);  
  StopRecord();
}

void StopRecord(){
  lightsOn = false;

  digitalWrite(hapticPin, LOW);
  
  savingCounter = 5;
  while(savingCounter > 0){
    if(!lightsOn) {
      lightUpAllLights(pixels.Color(255, 150, 0), 50); 
    } else {
      lightUpAllLights(pixels.Color(0, 0, 0), 25);  
    }
    lightsOn = !lightsOn;   
    delay(250);
    savingCounter--;
  }

  lightUpAllLights(pixels.Color(0, 0, 0), 0);  
  lightUpAllLights(pixels.Color(0, 255, 0), 50);
  delay(500);
  lightUpAllLights(pixels.Color(0, 0, 0), 50);  
  delay(500);
  lightUpAllLights(pixels.Color(0, 255, 0), 50);  
  delay(500);
  lightUpAllLights(pixels.Color(0, 0, 0), 50);  

  
  selectColor();
}

void readIncomingMessage() {
    if(Serial.available()) {
    reset();
    messageReceived = true;
    audioPlayed = false;

    for(int i = 0; i < 3; i++) {
      arr[i] = Serial.read();
    }
  
    redValue = arr[0];
    greenValue = arr[1];
    blueValue = arr[2];
  }
}

void listenToMessage() {
  messageReceived = false;
  setColor(redValue, greenValue, blueValue);
  runHapticFeedback();

  delay(5000); // simulates listening to the recording.
  audioPlayed = true;
  
  runHapticFeedback();
  setColor(0,0,0);
  reset();
}

void pulsateLights() {
  int iter = 0;
  int iterEnd = 50;
  int totalPulseTime = 1500;
  int iterWaitTime = 30;

  int fadeRed = (redValue/iterEnd)*2;
  int fadeGreen = (greenValue/iterEnd)*2;
  int fadeBlue = (blueValue/iterEnd)*2;
    
  while(iter < iterEnd) {

    tempRed = tempRed + fadeRed;
    tempGreen = tempGreen + fadeGreen;
    tempBlue = tempBlue + fadeBlue;

    int red = tempRed;
    int green = tempGreen;
    int blue = tempBlue;

    if(tempRed <= 0 || tempRed >= redValue) {
      if(tempRed >= 255) {
        red = 255/2;
      } else if(tempRed >= redValue) {
        red = tempRed/2;
      }
      fadeRed = -fadeRed;
    }
    
    if(tempGreen <= 0 || tempGreen >= greenValue) {
      if(tempGreen >= 255) {
        green = 255/2;
      } else if(tempGreen >= greenValue) {
        green = tempGreen/2;
      }
      fadeGreen = -fadeGreen;
    }
    
    if(tempBlue <= 0 || tempBlue >= blueValue) {
      if(tempBlue >= 255) {
        blue = 255/2;
      } else if(tempBlue >= blueValue) {
        blue = tempBlue/2;
      }
      fadeBlue = -fadeBlue;
    }

    setColor(red, green, blue);
    
    delay(iterWaitTime);
    iter++;
  }
}

void runHapticFeedback() {
  digitalWrite(hapticPin, HIGH);
  delay(100);
  digitalWrite(hapticPin, LOW);
}

void setColor(int red, int green, int blue) {
  pixels.clear();
  for(int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
}

void lightUpAllLights(uint32_t color, int wait){
  pixels.clear();
  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void reset() {
    redValue = 0;
    greenValue = 0;
    blueValue = 0;
    tempRed = 0;
    tempGreen = 0;
    tempBlue = 0;
    setColor(redValue,greenValue, blueValue);
}

// TUVAs function
void selectColor() {
  bool colorSelected = false;
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
    
    lightUpAllLights(pixels.Color(R, G, B), 0);
    bendVal = analogRead(bendPin);

    if(user == "Tuva") {
      // Working for Tuva
      if(bendVal > bendThreshold){ 
        colorSelected = true;
      }
    } else {
      if(bendVal < bendThreshold){ 
        colorSelected = true;
      }
    }
    delay(100);

  }
  if (colorSelected){
    vibrate(); 
    adjustColor(R, G, B);
  }
  int color[] = {R, G, B};
  sendMessage(color);
}

// MARIEs function
void sendMessage(int color[]) {
  // your code
  bool messageReady = true;

  while(messageReady){
    mpu6050.update();
    if(getTotalAcc() > throwThreshold){
      //Ball thrown
      sendData(color);
      messageReady = false;
    }else if(getTotalAcc() < dropThreshold){
      //Ball dropped
      reset();
      messageReady = false;
    }
  }
}

void adjustColor (int & R, int & G, int & B){
  int newR, newG, newB;
  bool colorAdjusted = false;
  float x, y, z;

  while(!colorAdjusted){
    mpu6050.update();
    x = limitValue(mpu6050.getAngleX());
    y = limitValue(mpu6050.getAngleY());
    z= limitValue(mpu6050.getAngleZ());
    
    int totalMovement = ((x + y + z) / 3)* 10; //Gets the average movement
    int totalValue = map(totalMovement, -360, 360, 1, 19);
    float precentage = totalValue / 10.0;
    newR = limitRGBvalue(R*precentage);
    newG = limitRGBvalue(G*precentage);
    newB = limitRGBvalue(B*precentage);
    lightUpAllLights(pixels.Color(newR, newG, newB), 0);
    delay(100);

    bendVal = analogRead(bendPin);

    if(user == "Tuva") {
      //Working for Tuva
      if(bendVal > bendThreshold){ 
        colorAdjusted = true;
      }
    } else {
      if(bendVal < bendThreshold){ 
        colorAdjusted = true;
      }
    }
  }
  if (colorAdjusted){
    vibrate(); 
    R = newR;
    G = newG;
    B = newB;
  }
}

void vibrate (){
  digitalWrite(hapticPin, HIGH);
  delay(400);
  digitalWrite(hapticPin, LOW); 
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

int limitRGBvalue(int value){
  return constrain(value, 10, 255); //Restrcit from not going to 0
}

float getTotalAcc(){
  float x = mpu6050.getAccX();
  float y = mpu6050.getAccY();
  float z = mpu6050.getAccZ(); 
  return sqrt(x*x + y*y + z*z);
}

void sendData(int color[]){
  delay(1000);
  runHapticFeedback();
  delay(100);
  runHapticFeedback();
  String payload = "message:" + String(color[0]) + "," + String(color[1]) + "," + String(color[2]);
  Serial.println(payload);
  reset();
}
