#include <Adafruit_NeoPixel.h>

#define LEDPin 6
#define numberOfPixels 8

Adafruit_NeoPixel pixels(numberOfPixels, LEDPin, NEO_GRB + NEO_KHZ800);

int hapticPin = 7;
bool audioPlayed = false;

int bendPin = A0;
int bendVal = 0;
int bendThreshold = 420;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int tempRed = 0;
int tempGreen = 0;
int tempBlue = 0;
int arr[] = {redValue, greenValue, blueValue};

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pinMode(hapticPin, OUTPUT);
  pinMode(bendPin, INPUT);
}
void loop() {
  
  if(Serial.available()) {
    reset();
    audioPlayed = false;

    for(int i = 0; i < 3; i++) {
      arr[i] = Serial.read();
    }
  
    redValue = arr[0];
    greenValue = arr[1];
    blueValue = arr[2];
  }
  
  bendVal = analogRead(bendPin);
  if(bendVal < bendThreshold) {
    audioPlayed = true;
    setColor(redValue, greenValue, blueValue);
    runHapticFeedback();
    
    while(bendVal < bendThreshold) {
      bendVal = analogRead(bendPin);
      // just delaying the code until audio is implemented into the code.
    }
    
    runHapticFeedback();
    setColor(0,0,0);
    reset();
  }

  if(!(arr[0] == 0 && arr[1] == 0 && arr[2] == 0) && !audioPlayed) {
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
}

void runHapticFeedback() {
  digitalWrite(hapticPin, HIGH);
  delay(40);
  digitalWrite(hapticPin, LOW);
}

void setColor(int red, int green, int blue) {
  pixels.clear();
  for(int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
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
