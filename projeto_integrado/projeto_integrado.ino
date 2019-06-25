#include <MozziGuts.h>
#include <Oscil.h>
//#include <ADSR.h>
#include <tables/sin2048_int8.h>
#include "pinout.h"
#define CONTROL_RATE 64
#define AUDIO_RATE 4096

#define DEBUG                   1
#define NUMBER_OF_BUTTONS       5
//#define KEY_PRESSED             HIGH

#define KEY_PRESSED_DELAY       256

byte buttons_pins[NUMBER_OF_BUTTONS];
int buttons_state[NUMBER_OF_BUTTONS];
int timers[NUMBER_OF_BUTTONS];
float state_frequencies[NUMBER_OF_BUTTONS];
float frequency_values[NUMBER_OF_BUTTONS];
Oscil<2048, AUDIO_RATE> osc[NUMBER_OF_BUTTONS];

float base_frequency = 440;

void setup() {
#if DEBUG
  Serial.begin(9600);
#endif
    frequency_values[0] = 261.63;
    frequency_values[1] = 293.66;
    frequency_values[2] = 329.63;
    frequency_values[3] = 349.23;
    frequency_values[4] =  392;

  initialize();
  startMozzi(CONTROL_RATE);
  adcReconnectAllDigitalIns();
}

void updateControl() {
  updateButtons();
 }

int updateAudio() {
  return ( buttons_state[0] * osc[0].next()
         + buttons_state[1] * osc[1].next()
         + buttons_state[2] * osc[2].next()
         + buttons_state[3] * osc[3].next()
         + buttons_state[4] * osc[4].next()
) >> 3;
}

void loop() {
  audioHook();
}

void updateButtons() {
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    
    if (digitalRead(buttons_pins[i]) == 1) {
      timers[i] = 50;
      buttons_state[i] = 1;
    }

   if (timers[i] == 0)
     buttons_state[i] = 0;

    if (timers[i] > 0)
      timers[i]--;
}   
    #if DEBUG
    Serial.print(":");
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
      Serial.print(" ");
      Serial.print(buttons_state[i]);
    }
    Serial.println();
    #endif   
}

void initialize() {
  assign_pins();
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    pinMode(buttons_pins[i], INPUT);
  //  buttons_state[i] = !KEY_PRESSED;
    buttons_state[i] = 0;
    setFrequencies(i);
  //  setEnvelopes(i);
  }
}

void setFrequencies(int i) {
 // frequency_values[i] = base_frequency * pow(2, i / 12.0);
 
  osc[i].setTable(SIN2048_DATA);
  osc[i].setFreq(frequency_values[i]);
}


void assign_pins() {
  buttons_pins[0] = KEY_0_PIN;
  buttons_pins[1] = KEY_1_PIN;
  buttons_pins[2] = KEY_2_PIN;
  buttons_pins[3] = KEY_3_PIN;
  buttons_pins[4] = KEY_4_PIN;

}
