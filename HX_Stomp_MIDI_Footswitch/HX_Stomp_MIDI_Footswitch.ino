#include <MIDI.h>

// : --- MIDI controller for LINE6 HX Stomp ---
// : Supports 2 Switches and 3 input controls (FS4, FS5, Tuner)
// : Uses MIDI CC# 52, 53 and 68 to mulates FS4, FS5 and Tuner

const int fs4 = 2;                      // Connect Switch to D2 (Pull down, Normally Open)
const int fs5 = 3;                      // Connect Switch to D3 (Pull down, Normally Open)
unsigned long lastDebounceTime = 0;     // the last time the switch state changed
unsigned long debounceDelay = 80;       // the debounce time (ms)
int prevButtonState[2] = { LOW, LOW };  // Save the previous switch state
int buttonState[2];                     // Save the stable switch state

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(1);  // using MIDI Channel 1
  pinMode(fs4, INPUT);
  pinMode(fs5, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(4, LOW);
}

void loop() {
  int tempReading[2] = { digitalRead(fs4), digitalRead(fs5) };
  // for switch state indicator LEDs
  digitalWrite(4, tempReading[0]);  
  digitalWrite(5, tempReading[1]);
  //Check input change & set timer
  if ((tempReading[0] != prevButtonState[0]) || (tempReading[1] != prevButtonState[1])) {
    lastDebounceTime = millis();
  }

  //Take input changes as stable input after threshold
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if ((tempReading[0] != buttonState[0]) || (tempReading[1] != buttonState[1])) {
      buttonState[0] = tempReading[0];
      buttonState[1] = tempReading[1];
      sendMessage();
    }
  }

  prevButtonState[0] = tempReading[0];
  prevButtonState[1] = tempReading[1];
}

void sendMessage() {
  //Transmit actual MIDI message
  if ((buttonState[0] == HIGH) && (buttonState[1] == LOW)) {  //FS4
    MIDI.sendControlChange(52, 1, 1);
  }
  if ((buttonState[0] == LOW) && (buttonState[1] == HIGH)) {  //FS5
    MIDI.sendControlChange(53, 1, 1);
  }
  if ((buttonState[0] == HIGH) && (buttonState[1] == HIGH)) {  //Tuner
    MIDI.sendControlChange(68, 1, 1);
  }
}