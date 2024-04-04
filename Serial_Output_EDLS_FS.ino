#include <Bounce.h>
#include <MIDI.h>

/******************************
   EDLS-FS v1.0
   for Teensy LC (www.pjrc.com)
   by Notes and Volts
   www.notesandvolts.com
 ******************************/

/******************************
   ** Upload Settings **
   Board: "Teensy LC"
   USB Type: "Serial"
   CPU Speed: "48 Mhz"
 ******************************/

//Switch CC Numbers
#define LOOP1 38
#define LOOP2 39
#define LOOP4 40
#define LOOP8 41

//Pin Numbers on Teensy
#define L1_PIN 0
#define L2_PIN 1
#define L4_PIN 2
#define L8_PIN 3

//Other
#define SWITCHES 4 // How many switches?
#define VELOCITY 127 // ON Velocity
#define BOUNCE_TIME 15 // Debounce Time (in milliseconds)

//********************

//Global variables
const int DIGITAL_SWITCHES[SWITCHES] = {L1_PIN, L2_PIN, L4_PIN, L8_PIN};
const int CC_NUMBER[SWITCHES] = {LOOP1, LOOP2, LOOP4, LOOP8};
const int channel = 1; // MIDI channel

//Set up 'Bounce' Objects
Bounce digital[] =   {
  Bounce(DIGITAL_SWITCHES[0], BOUNCE_TIME),
  Bounce(DIGITAL_SWITCHES[1], BOUNCE_TIME),
  Bounce(DIGITAL_SWITCHES[2], BOUNCE_TIME),
  Bounce(DIGITAL_SWITCHES[3], BOUNCE_TIME),
};

// Initialize the MIDI library
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

//Setup
void setup() {
  Serial2.begin(31250); // Initialize MIDI serial communication
  MIDI.begin(channel); // Initialize MIDI library with the specified channel
  for (int i = 0; i < SWITCHES; i++) {
    pinMode(DIGITAL_SWITCHES[i], INPUT_PULLUP);
  }
}

//Main Loop
void loop() {
  readSwitches();
}

//Read Buttons
void readSwitches() {
  static byte input = 0;

  digital[input].update();

  if (digital[input].fallingEdge()) {
    MIDI.sendControlChange(CC_NUMBER[input], VELOCITY, channel); // Send MIDI control change message
     Serial.print("Switch ");
    Serial.print(input + 1);
    Serial.println(" pressed"); // Print debug message for switch pressed
  }
  else if (digital[input].risingEdge()) {
    MIDI.sendControlChange(CC_NUMBER[input], 0, channel); // Send MIDI control change message (turn off)
       Serial.print("Switch ");
    Serial.print(input + 1);
    Serial.println(" released"); // Print debug message for switch pressed
  }

  input++;
  if (input >= SWITCHES) input = 0;
}
