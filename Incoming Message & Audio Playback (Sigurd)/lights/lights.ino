int LEDRedPin = 6;
int LEDGreenPin = 5;
int LEDBluePin = 3;

String serialLine = "none";
String recentCommand = "";

int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int tempRed = 0;
int tempGreen = 0;
int tempBlue = 0;

void setup() {
  pinMode(LEDRedPin, OUTPUT);
  pinMode(LEDGreenPin, OUTPUT);
  pinMode(LEDBluePin, OUTPUT);

  Serial.begin(115200);
}
void loop() {
  
  if(Serial.available()) {

    serialLine = Serial.readString();
    Serial.println(serialLine);
    if(recentCommand != serialLine || serialLine != "") {
      recentCommand = serialLine;

      int firstDelimiter = 0;
      int lastDelimiter = 0;
      for(int i = 0; i < 3; i++) {
        lastDelimiter = recentCommand.indexOf(",", firstDelimiter);
        switch(i) {
          case 0:
            redValue = recentCommand.substring(firstDelimiter, lastDelimiter).toInt();
            break;
          case 1:
            greenValue = recentCommand.substring(firstDelimiter, lastDelimiter).toInt();
            break;
          case 2:
            blueValue = recentCommand.substring(firstDelimiter, recentCommand.length()+1).toInt();
            break;
        }
        firstDelimiter = lastDelimiter+1;
      }
    }
  }
  
  if(serialLine != "none") {
    int iter = 0;
    int iterEnd = 50;
    int totalPulseTime = 1500;
    int iterWaitTime = 30;

    int fadeRed = (redValue/iterEnd)*2;
    int fadeGreen = (greenValue/iterEnd)*2;
    int fadeBlue = (blueValue/iterEnd)*2;
      
    while(iter < iterEnd) {
      analogWrite(LEDRedPin, tempRed);
      analogWrite(LEDGreenPin, tempGreen);
      analogWrite(LEDBluePin, tempBlue);

      tempRed = tempRed + fadeRed;
      tempGreen = tempGreen + fadeGreen;
      tempBlue = tempBlue + fadeBlue;

      if(tempRed <= 0 || tempRed >= redValue) {
        analogWrite(LEDRedPin, 0);
        fadeRed = -fadeRed;
      }
      if(tempGreen <= 0 || tempGreen >= greenValue) {
        analogWrite(LEDGreenPin, 0);
        fadeGreen = -fadeGreen;
      }
      if(tempBlue <= 0 || tempBlue >= blueValue) {
        analogWrite(LEDBluePin, 0);
        fadeBlue = -fadeBlue;
      }
      Serial.println(tempRed);
      delay(iterWaitTime);
      iter++;
    }
  }
}

//      int pulseCounter = 0;
//      int pulseCounterEnd = (totalWaitTime / loopWaitTime);
//      int offsetRed = redValue / ((pulseCounterEnd -1 ) /2);
//      int offsetGreen = greenValue / ((pulseCounterEnd -1) /2);
//      int offsetBlue = blueValue / ((pulseCounterEnd -1) /2);
//      
//      while(pulseCounter < pulseCounterEnd) {
//        if(pulseCounter > pulseCounterEnd/2-1 ) {
//          Serial.println("over 10");
//          // adjust color back to original
//          setColor(
//            0 + (offsetRed * (pulseCounter /2)), 
//            0 + (offsetGreen * (pulseCounter /2)), 
//            0 + (offsetBlue * (pulseCounter /2))
//          );
//        } else {
//          Serial.println("under 10");
//          // adjust colour down to 0, 0, 0
//          setColor(
//            redValue - (offsetRed * pulseCounter), 
//            greenValue - (offsetGreen * pulseCounter), 
//            blueValue - (offsetBlue * pulseCounter)
//          );
//        }
//        delay(loopWaitTime);
//        pulseCounter++;
//      }
//    }
//  }
  
//  RGB_color(255, 0, 0); // Red
//  delay(1000);
//  RGB_color(0, 255, 0); // Green
//  delay(1000);
//  RGB_color(0, 0, 255); // Blue
//  delay(1000);
//  RGB_color(255, 255, 125); // Raspberry
//  delay(1000);
//  RGB_color(0, 255, 255); // Cyan
//  delay(1000);
//  RGB_color(255, 0, 255); // Magenta
//  delay(1000);
//  RGB_color(255, 255, 0); // Yellow
//  delay(1000);
//  RGB_color(255, 255, 255); // White
//}

void setColor(int redValue, int greenValue, int blueValue)
 {
  analogWrite(LEDRedPin, redValue);
  analogWrite(LEDGreenPin, greenValue);
  analogWrite(LEDBluePin, blueValue);
}
