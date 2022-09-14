#include <MIDI.h>

// : --- MIDI controller for LINE6 HX Stomp ---
// : Supports 4 momentary witches (FS4, FS5, Tuner, Tap) and 1 expression pedal (EXP2)

// Switch connection
const byte sw1 = 2;                      // Connect Switch1 to D2 (Pull down, Normally Open, pressed=HIGH)
const byte sw2 = 3;                      // Connect Switch2 to D3 (Pull down, Normally Open, pressed=HIGH)
const byte sw3 = 4;                      // Connect Switch3 to D4 (Pull down, Normally Open, pressed=HIGH)
const byte sw4 = 5;                      // Connect Switch4 to D5 (Pull down, Normally Open, pressed=HIGH)

// EXP pedal connection
const byte expedal = A0;

// LED connection: digital pin 6 - 9
const byte led1 = 6;
const byte led2 = 7;
const byte led3 = 8;
const byte led4 = 9;

// MIDI CC# for HX Stomp setting
const byte fs4 = 52;    // Foot Switch 4
const byte fs5 = 53;    // Foot Switch 5
const byte tuner = 68;  // Tuner screen on/off
const byte tap = 64;    // Tap tempo
const byte pedal = 2;   // Expression Pedal 2

// Variables for input debouncing
unsigned long lastDebounceTime = 0;     // the last time the switch state changed
unsigned long debounceDelay = 50;       // the debounce time (ms)
byte prevButtonState = 0;          // Saves the previous switch state to each bit: bit 0(LSB) = SW1 ~ bit 3 = SW4
byte buttonState;                       // Saves the stable switch state

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(1);  // using MIDI Channel 1
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(sw3, INPUT);
  pinMode(sw4, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
}

void loop() {                                     // tempReading = 0b00000000 : stores the raw inputs from switches to each bits.
  byte tempReading = 0;                           //                     ||||
  bitWrite(tempReading, 0, digitalRead(sw1));     //                     |||+- SW1
  bitWrite(tempReading, 1, digitalRead(sw2));     //                     ||+-- SW2
  bitWrite(tempReading, 2, digitalRead(sw3));     //                     |+--- SW3
  //bitWrite(tempReading, 3, digitalRead(sw4));     //                     +---- SW4

  // for switch state indicator LEDs
  digitalWrite(led1, bitRead(tempReading, 0));
  digitalWrite(led2, bitRead(tempReading, 1));
  digitalWrite(led3, bitRead(tempReading, 2));
  digitalWrite(led4, bitRead(tempReading, 3));
  //Serial.println(tempReading);
  //Check input change & set timer
  if (tempReading != prevButtonState) {
    lastDebounceTime = millis();
    
  }
  
  //Take input changes as stable input after threshold
  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    if (tempReading != buttonState) {
      buttonState = tempReading;
      sendMessage();
      
    }
  }
  prevButtonState = tempReading;
  //sendPedalMessage();         // will work on this...
}

void sendMessage() {
  //Transmit actual MIDI message of foot switch control
  byte control;
  switch (buttonState) {
    case 1:             //0b0001
      control = fs4;
      break;
    case 2:             //0b0010
      control = fs5;
      break;
    case 4:             //0b0100
      control = tuner;
      break;
    case 8:             //0b1000
      control = tap;
      break;
    default:
      return;           //Do nothing and escape
  } 
  MIDI.sendControlChange(control, 64, 1); // since the value 64 is acceptable to all 4 controls...
}

void sendPedalMessage() {
  //Transmit MIDI message of expression pedal control.. NOT TESTED!!
  int pedalReading = analogRead(expedal);
  MIDI.sendControlChange(pedal, map(pedalReading, 0, 1023, 0, 127), 1);
}
