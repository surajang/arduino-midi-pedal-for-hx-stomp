#include <MIDI.h>

// : --- MIDI controller for LINE6 HX Stomp ---
// : Supports 4 momentary witches (FS4, FS5, Tuner, Tap) and 1 analog expression pedal (EXP2)

// Define switch - digital pin connections
const byte sw1 = 2;                      // Connect Switch1 to D2 (Pull down, Normally Open, pressed=HIGH)
const byte sw2 = 5;                      // Connect Switch2 to D5 (Pull down, Normally Open, pressed=HIGH)
const byte sw3 = 4;                      // Connect Switch3 to D4 (Pull down, Normally Open, pressed=HIGH)
const byte sw4 = 3;                      // Connect Switch4 to D3 (Pull down, Normally Open, pressed=HIGH)

// EXP pedal connection
const byte expedal = A6;

// LED connection: digital pin 10, 11
const byte led1 = 10;
const byte led2 = 11;

// MIDI CC# for HX Stomp setting
const byte fs4 = 52;    // Foot Switch 4
const byte fs5 = 53;    // Foot Switch 5
const byte tuner = 68;  // Tuner screen on/off
const byte tap = 64;    // Tap tempo
const byte pedal = 2;   // Expression Pedal 2

// Variables for input debouncing
unsigned long lastDebounceTime = 0;     // the last time the switch state changed
unsigned long debounceDelay = 50;       // the debounce time (ms)
byte prevButtonState = 0;               // Saves the previous switch state to each bit: bit 0(LSB) = SW1 ~ bit 3 = SW4
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
  digitalWrite(led1, HIGH); // POWER status LED
}

void loop() {                                     // tempReading = 0b00000000 : stores the raw inputs from switches to each bits.
  byte tempReading = 0;                           //                     ||||
  bitWrite(tempReading, 0, digitalRead(sw1));     //                     |||+- SW1
  bitWrite(tempReading, 1, digitalRead(sw2));     //                     ||+-- SW2
  bitWrite(tempReading, 2, digitalRead(sw3));     //                     |+--- SW3
  bitWrite(tempReading, 3, digitalRead(sw4));     //                     +---- SW4

  // for switch state indicator LEDs
  if(tempReading != 0) {
    digitalWrite(led2, HIGH); // Active status LED
  } else {
    digitalWrite(led2, LOW);
  }

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
      control = fs5;
      break;
    case 2:             //0b0010
      control = fs4;
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
  //Transmit MIDI message of expression pedal control
  int pedalReading = analogRead(expedal);
  MIDI.sendControlChange(pedal, map(pedalReading, 0, 1023, 0, 127), 1);
}
