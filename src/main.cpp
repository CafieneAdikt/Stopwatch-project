#include <Arduino.h>

#include "SevSeg.h"
SevSeg sevseg; // Instantiate a seven segment controller object

const int INTBUTTON = 1;             // interupt button
volatile bool buttonPressed = false; // flag for button
const unsigned long DEBOUNCE_TIME_MS = 200;
unsigned long previousDebounce = 0;
unsigned long previousMillis = 0;
bool counter_running = true; // flag for counter

unsigned int deciSeconds = 0;

void Buttonpressed_ISR(); // define the ISR

void setup() {
  Serial.begin(115200);

  byte numDigits = 4;
  byte digitPins[] = {17, 19, 15, 13};
  byte segmentPins[] = {7, 6, 10, 3, 9, 4, 2, 12};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = N_TRANSISTORS; // See README.md for options
  bool updateWithDelays = false;       // Default. Recommended
  bool leadingZeros =
      false; // Use 'true' if you'd like to keep the leading zeros
  pinMode(INTBUTTON, INPUT_PULLUP); // interupt button
  attachInterrupt(digitalPinToInterrupt(INTBUTTON), Buttonpressed_ISR, FALLING);

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins,
               resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(100);
}

void loop() {
unsigned long currentMillis = millis();

if(counter_running == true){
  if ((currentMillis - previousMillis) >= 100) {
    deciSeconds++;
    previousMillis += 100;
    previousMillis = currentMillis;   // set new debounce time
  }
}
  sevseg.setNumber(deciSeconds, 1);

  sevseg.refreshDisplay(); // Must run repeatedly
}

void Buttonpressed_ISR() {
  // send flag high
  buttonPressed = true;

  unsigned long currentmillis = millis(); // recheck the time

  if (buttonPressed &&
      ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
    previousDebounce = currentmillis;   // set new debounce time
    counter_running = !counter_running; // toggles state
    buttonPressed = false;
  }
}
