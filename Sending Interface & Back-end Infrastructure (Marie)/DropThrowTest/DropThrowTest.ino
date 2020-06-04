#include <MPU6050_tockn.h>
#include <Wire.h>

//Setup accelerometer with MPU library
MPU6050 mpu6050(Wire);


float dropThreshold = 0.30;
float throwThreshold = 1.50;
float acc;

void setup() {
  
  Serial.begin(115200);
  //Accelerometer setup
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}
void loop() {
  acc = getTotalAcc();
  mpu6050.update();
  if(acc > throwThreshold){
    dropThrow(false);
  }else if(acc < dropThreshold){
    dropThrow(true);
  }
}

void dropThrow(bool drop){
  long t = millis();
  
  while(millis() - t <  500){
    mpu6050.update();
    float acc = getTotalAcc();
    if(drop){
      if(acc > throwThreshold){
        Serial.println("DROP");
        break;
      }
    }else{
      if(acc < dropThreshold){
        Serial.println("THROW");
        break;
      } 
    }
  }
  accelCooldown();
  
}

void accelCooldown(){
  long t = millis();
  while(millis() - t < 1000){
    mpu6050.update();
  }
}
