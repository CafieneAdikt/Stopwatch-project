#include <Arduino.h>

#include "SevSeg.h"
SevSeg sevseg; // Instantiate a seven segment controller object

const int INTBUTTON = 1;             // stop start button
const int INTBUTTON2 = 0;            // reset button
volatile bool buttonPressed = false; // flag for button
volatile bool resetPressed = false; // flag for reset button
const unsigned long DEBOUNCE_TIME_MS = 200; // debounce time interval
unsigned long previousDebounce = 0; // define and set variable to zero
unsigned long previousMillis = 0; // define and set variable to zero
bool counter_running = true; // flag for counter

unsigned int deciSeconds = 0; // define variable

void Buttonpressed_ISR(); // define the ISR

void ResetButton_ISR(); // define interrupt routine

void setup() {
  
  byte numDigits = 4; // number of digits in segment display
  byte digitPins[] = {17, 19, 15, 13}; // cathode pin numbers
  byte segmentPins[] = {7, 6, 10, 3, 9, 4, 2, 12}; // segment pin numbers
  bool resistorsOnSegments = false; 
  byte hardwareConfig = N_TRANSISTORS;
  bool updateWithDelays = false;  
  bool leadingZeros = false;

  pinMode(INTBUTTON, INPUT_PULLUP); // interupt button
  pinMode(INTBUTTON2, INPUT_PULLUP); // reset button
  attachInterrupt(digitalPinToInterrupt(INTBUTTON), Buttonpressed_ISR, FALLING); // set up interrupt routines
  attachInterrupt(digitalPinToInterrupt(INTBUTTON2), ResetButton_ISR, FALLING);

  // setup for seven segment display
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(100);
}

void loop() {
unsigned long currentMillis = millis();

// if statement so the interrupt can stop or start counting
if(counter_running == true){
  // this statement means that if move that 100 milliseconds has elapsed increase deciseconds by 1  
  if ((currentMillis - previousMillis) >= 100) { 
    deciSeconds++;
    previousMillis += 100;
    previousMillis = currentMillis;   // set the new time 
  }
}
  sevseg.setNumber(deciSeconds, 1); // set number to be displayed

  sevseg.refreshDisplay(); // Must run repeatedly
}

void Buttonpressed_ISR() {
  // send flag high so that the following if statement is true
  buttonPressed = true;

  unsigned long currentmillis = millis(); // recheck and set the time variable

  // debounce the button 
  if (buttonPressed &&
      ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
    previousDebounce = currentmillis;   // set new debounce time
    counter_running = !counter_running; // toggles state
    buttonPressed = false; // reset the flag
  }
}

void ResetButton_ISR(){
  // send flag high so the following if statement is true
  resetPressed = true;

  unsigned long currentmillis = millis(); // recheck and set the time variable
  
  // debounce button
  if (resetPressed &&
      ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
    previousDebounce = currentmillis;   // set new debounce time
    deciSeconds = 0; // reset the variable in the main loop back to zero
    resetPressed = false; // reset the flag
  }
}
