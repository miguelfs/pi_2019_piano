#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include "pinout.h"

#define CONTROL_RATE            128
#define DEBUG                   1
#define NUMBER_OF_BUTTONS       6
#define KEY_ACTIVE              LOW

int buttons_pins[NUMBER_OF_BUTTONS];
int buttons_state[NUMBER_OF_BUTTONS];
float frequency_multipliers[NUMBER_OF_BUTTONS];
float frequency_values[NUMBER_OF_BUTTONS];

float base_frequency = 880;

Oscil<2048, AUDIO_RATE> osc[NUMBER_OF_BUTTONS];

void setup() {
  #if DEBUG
  Serial.begin(9600);
  #endif
  
  initialize();
  startMozzi(CONTROL_RATE); 
}

void updateControl(){
  updateButtons();
}

void loop(){
  audioHook();
}

// updateAudio:
// Returns sound played, considering polyphonic tunes.
// Since for loop screws with computational time, the best choice is to sum all the values, multiplied by their button state (1 or 0);
// Bitwise operation used for fast division in order to decrease output saturation.
int updateAudio(){
  return ( buttons_state[0] * osc[0].next() + buttons_state[1] * osc[1].next() +
  buttons_state[2] * osc[2].next() + buttons_state[3] *osc[3].next() +
  buttons_state[4]*osc[4].next() + buttons_state[5]*osc[5].next()) >> 2;
}

void updateButtons() {
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    if (digitalRead(buttons_pins[i]) == KEY_ACTIVE) {
      // 1 -> Button pressed, 0 otherwise
      buttons_state[i] = !KEY_ACTIVE;
  }
    else
      buttons_state[i] = KEY_ACTIVE;
  }
}

void initialize() {
  buttons_pins[0] = KEY_0_PIN;
  buttons_pins[1] = KEY_1_PIN;
  buttons_pins[2] = KEY_2_PIN;
  buttons_pins[3] = KEY_3_PIN;
  buttons_pins[4] = KEY_4_PIN;
  buttons_pins[5] = KEY_5_PIN;
  
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    pinMode(buttons_pins[i], INPUT);
    
    buttons_state[i] = KEY_ACTIVE;

    frequency_multipliers[i] = pow(2, i/12.0);
    frequency_values[i] = base_frequency * frequency_multipliers[i];
    osc[i].setTable(SIN2048_DATA);
    osc[i].setFreq(frequency_values[i]);
  }
}
