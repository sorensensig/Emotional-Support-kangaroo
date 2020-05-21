#include <Adafruit_NeoPixel.h>

#define SensorLED     13
#define SensorINPUT   3  //Connect the sensor to digital Pin 3 which is Interrupts 1.

unsigned char state = 0;

#define PIN        6
#define NUMPIXELS 4

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pinMode(SensorLED, OUTPUT);
  pinMode(SensorINPUT, INPUT);
  attachInterrupt(1, blink, FALLING);// Trigger the blink function when the falling edge is detected

  pixels.begin();
  pixels.clear();
  pixels.show();

  Serial.begin(9600);

  Serial.print("test");
}
void loop()
{
      if(state!=0)
      {
        state = 0;
        digitalWrite(SensorLED,HIGH);

        long red = randomColour();
        long blue = randomColour();
        long green = randomColour();

        for(int i=0; i<NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(red, green, blue));
          pixels.show();
        }
        delay(500);
      }
      else 
      {
        digitalWrite(SensorLED,LOW);
        pixels.clear();
      }
        
}


void blink()//Interrupts function
{
  state++;
}

long randomColour() {;
  long randNumber = random(255);
  Serial.println(randNumber);
  return randNumber;
}
