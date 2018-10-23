#include <Arduino.h>

#include "SevSeg.h"
SevSeg sevseg; //Instantiate a seven segment controller object

const int INTBUTTON = 1;            // interupt button
volatile bool buttonPressed = false; // flag for button
const unsigned long DEBOUNCE_TIME_MS = 200;
unsigned long previousDebounce = 0; 
bool counter_running = true; // flag for counter

void Buttonpressed_ISR(); // define the ISR

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {17, 19, 15, 13};
  byte segmentPins[] = {7, 6, 10, 3, 9, 4, 2, 12};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = N_TRANSISTORS; // See README.md for options
  bool updateWithDelays = false; // Default. Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  pinMode(INTBUTTON, INPUT_PULLUP); // interupt button
  attachInterrupt(digitalPinToInterrupt(INTBUTTON), Buttonpressed_ISR, FALLING);
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(90);
}

void loop() {
  static unsigned long timer = millis();
  static int deciSeconds = 0;
  
  
  if (millis() >= timer) {
    deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond
    timer += 100; 
    if (deciSeconds == 10000) { // Reset to 0 after counting for 1000 seconds.
      deciSeconds=0;
    }
    sevseg.setNumber(deciSeconds, 1);
  }
  if(counter_running == true){
  sevseg.refreshDisplay(); // Must run repeatedly
  }
}

void Buttonpressed_ISR() {
  // send flag high
  buttonPressed = true;

  unsigned long currentmillis = millis(); //recheck the time

  if (buttonPressed &&
      ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
    previousDebounce = currentmillis; // set new debounce time
    counter_running = !counter_running; // toggles state
    buttonPressed = false;

  }
 
}
