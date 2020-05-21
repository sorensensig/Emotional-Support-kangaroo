#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIXEL_PIN    6  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 8  // Number of NeoPixels

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

const int buttonPin = 10;

int pixelNumber;

int buttonRead;
int buttonReadTwo;
const int flexPin = A1; //pin A0 to read analog input
const int vibrate = 2;
bool vibrating = false;

int redPower = 0;
int yellowPower = 0;
bool on;

int savingCounter = 0;
int fadingNumber = 5;
int fadingNumberTwo = 255;

bool isRecording = false;

int saveValue; //save analog value
int flexValue;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(A1,INPUT);
  pinMode(vibrate, OUTPUT);


  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  strip.clear();
  lightUpAllLights(strip.Color(0, 0, 0), 0);  

}


void loop() {
   flexValue = analogRead(flexPin);         //Read and save analog value from potentiometer
  
  if(flexValue < 380 && !isRecording){
    isRecording = true;
    Record();
  }
}

void Record(){
  redPower = 0;
  pixelNumber = 0;
  on = false;
  Serial.println(1);
  
  vibrating = true;
  
  while(flexValue < 380  ){
    flexValue = analogRead(flexPin);         //Read and save analog value from potentiometer
    buttonRead = digitalRead(buttonPin);
    digitalWrite(vibrate, HIGH);
    delay(40);
    digitalWrite(vibrate, LOW);
    if(!on){
      lightUpAllLights(strip.Color(255, 0, 0), 100); 
    }else{
      lightUpAllLights(strip.Color(0, 0, 0), 50);  
    }
    on = !on;   
    delay(500);
  }
  lightUpAllLights(strip.Color(0, 0, 0), 0);  
  StopRecord();
}

void StopRecord(){
  yellowPower = 0;
  on = false;

  digitalWrite(vibrate, LOW);
  
  savingCounter = 5;
  yellowPower = 0;
  while(savingCounter > 0){
    if(!on){
      lightUpAllLights(strip.Color(255, 150, 0), 50); 
    }else{
      lightUpAllLights(strip.Color(0, 0, 0), 25);  
    }
    on = !on;   
    delay(250);
    savingCounter--;
  }

  lightUpAllLights(strip.Color(0, 0, 0), 0);  

  isRecording = false;
  lightUpAllLights(strip.Color(0, 255, 0), 50);
  delay(500);
  lightUpAllLights(strip.Color(0, 0, 0), 50);  
  delay(500);
  lightUpAllLights(strip.Color(0, 255, 0), 50);  
  delay(500);
  lightUpAllLights(strip.Color(0, 0, 0), 50);  
  
  Serial.println(0);
}


void lightUpAllLights(uint32_t color, int wait){
  strip.clear();
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
