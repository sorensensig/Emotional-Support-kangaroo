#include<Wire.h>
#include <Adafruit_NeoPixel.h>

const int LED_PIN = 14;
const int NUMPIXELS = 8;

const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float X, Y, Z, totalAccel;
float threshold = 20000;
bool haveBeenShaken = false;
int count = 0;
int randomColourCount = 0;
int vibrationPin = 33;

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

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  //Setup accelerometer
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);

  //Setup Neopixel LED
  pixels.begin();
  for(int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();
  }

  //Setup vibration sensor
  pinMode(vibrationPin, OUTPUT);
  
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();   

  X = 0;
  Y = 0;
  Z = 0;
  for (int i=0; i<10; i++) {
    X += AcX;
    Y += AcY;
    Z += AcZ;
    delay(1);
  }
  X /= 10;
  Y /= 10;
  Z /= 10; 

  totalAccel = sqrt(X*X + Y*Y + Z*Z);
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(X);
  Serial.print(" | Y = "); Serial.print(Y);
  Serial.print(" | Z = "); Serial.println(Z); 
  Serial.print("Total:"); Serial.println(totalAccel);
  Serial.println(" ");

  if(totalAccel > threshold){
    //Shake is detected and counting
    Serial.println("Over threshold"); //needs to be over threshold for several rounds. 
    count++;
    haveBeenShaken = true; 
    if (count > 2){
        //starting the vibration
      digitalWrite(vibrationPin, HIGH);
      delay(1000);
      digitalWrite(vibrationPin, LOW);   
    }
  }
  else if(haveBeenShaken && count > 2){
    //Shake has stopped
    int red, blue, green;
    //Determine the new colour
    if(randomColourCount <= 10){
      //Choose from 10 first random colours
      int colourNr = random(10); //Sets one of the colours as the chosen colour
      red = colours[colourNr][0];
      green = colours[colourNr][1];
      blue = colours[colourNr][2];
    }
    else{
      red = randomColour();
      blue = randomColour();
      green = randomColour();
    }

    //Setting the colours
    for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
      pixels.show();
    }

   

    //resetting the shake
    haveBeenShaken = false;
    count = 0;
  }
  else{
    count = 0;
    haveBeenShaken = false;
  }
  delay(333);
}

long randomColour() {;
  long randNumber = random(150);
  Serial.println(randNumber);
  return randNumber;
}
