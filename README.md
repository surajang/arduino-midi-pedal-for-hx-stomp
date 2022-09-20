# Arduino MIDI pedal for LINE6 HX Stomp

This repo contains source code for an Arduino sketch that emulates FS4 and 5 of LINE 6 HX Stomp via MIDI interface.

## What is this good for?
HX stomp is a great device for me though, it has only 3 footswitches(FS) so the control is quite limited. To use more FS you can plug in some 2-switch box to EXP/FS socket but if you're using this port for an expression pedal than have no luck. So the solution is using built-in MIDI interface and Arduino. You know, we are MAKERS :)

## HW Parts
- 1 x Arduino
  - Tested with Arduino UNO. Others with same spec. should work
- 4 x momentary switches
  - Bring your favorite pedal switches (any momentary on/off switch works)
- 2 x LEDs of your choice
  - Optional. One for power, the other indicates activation status
- 1 x 5-PIN DIN female MIDI socket
  - Good old MIDI interface socket
- 2 x 220 ohm resistors
  - These two resistors are required to connect MIDI socket to Arduino

## Circuit
- MIDI Socket
  - Arduino Pin 1(Tx) to Pin 5 of MIDI socket through 220ohm resistor
  - Arduino GND to Pin 2 of MIDI socket
  - Arduino + 5V to pin 4 of MIDI socket through 220ohm resistor
- Switches
  - Arduino Pin 2, 3, 4, 5 to each momentary switches that connect to ground
- Switch Indicator
  - Arduino Pin 10, 11 to each LEDs (use appropriate resistors)

## Arduino Library Used
- [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library)

## Features
- SW debouncing for 4 switch inputs for robust user experience
- Can recognise 4 inputs that emulate FS4, FS5, Tuner & tap tempo switch.

## DIYable Parts
- Change the CC# to control different feature of your MIDI device
- Tune the debounceDelay value for your switch of choice

## Todo
- Implement analog input feature
