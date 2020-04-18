// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin

// variables will change:
int buttonState = LOW;         // variable for reading the pushbutton status
int hasPressed = 0;           // Support variable to register ONE press of the button, even if its pushed down

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // For debugging purposes
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH && hasPressed == 0) {
    Serial.println("Hello World!");
    hasPressed = 1;
  } else if(buttonState == LOW) {
    hasPressed = 0;
  }

  /**
   * Handling a response on the serial bus
   */
  String a;
  while(Serial.available()){
    a += Serial.readString();
  }

  // For debuggin purposes
  if(a == "received"){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
