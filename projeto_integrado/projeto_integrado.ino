#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include "pinout.h"

#define CONTROL_RATE            128
#define DEBUG                   0
#define NUMBER_OF_BUTTONS       12
#define KEY_ACTIVE              LOW

int buttons_pins[NUMBER_OF_BUTTONS];
int buttons_state[NUMBER_OF_BUTTONS];
float frequency_multipliers[NUMBER_OF_BUTTONS];
float frequency_values[NUMBER_OF_BUTTONS];

float base_frequency = 880;

Oscil<2048, AUDIO_RATE> osc[NUMBER_OF_BUTTONS];

void setup() {
  #if DEBUG
  Serial.begin(115200);
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
  return ( buttons_state[0] * osc[0].next() 
          + buttons_state[1] * osc[1].next() 
          + buttons_state[2] * osc[2].next() 
          + buttons_state[3] * osc[3].next() 
          + buttons_state[4] * osc[4].next() 
          + buttons_state[5] * osc[5].next()
          + buttons_state[6] * osc[6].next() 
          + buttons_state[7] * osc[7].next() 
          + buttons_state[8] * osc[8].next() 
          + buttons_state[9] * osc[9].next() 
          + buttons_state[10] * osc[10].next()
          + buttons_state[11] * osc[11].next()
          ) >> 2;
}

void updateButtons() {

  #if DEBUG
  Serial.print("Estado dos botoes: ");
  #endif
  
  for (int i = 0; i < NUMBER_OF_BUTTONS-1; i++) {
    if (digitalRead(buttons_pins[i]) == KEY_ACTIVE) {
      // 1 -> Button pressed, 0 otherwise
      buttons_state[i] = !KEY_ACTIVE;
    }
    else
      buttons_state[i] = KEY_ACTIVE;

    #if DEBUG
    Serial.print(" ");
    Serial.print(buttons_state[i]);
    #endif
          
  }
  
  if (analogRead(buttons_pins[11]) < 10) {
    buttons_state[11] = !KEY_ACTIVE;
    
    #if DEBUG
    Serial.print(" ");
    Serial.print(buttons_state[11]);
    #endif
  }
  else
    buttons_state[11] = KEY_ACTIVE;
  
  #if DEBUG
  Serial.println();
  #endif
  
}

void initialize() {
  buttons_pins[0] = KEY_0_PIN;
  buttons_pins[1] = KEY_1_PIN;
  buttons_pins[2] = KEY_2_PIN;
  buttons_pins[3] = KEY_3_PIN;
  buttons_pins[4] = KEY_4_PIN;
  buttons_pins[5] = KEY_5_PIN;
  buttons_pins[6]  = KEY_6_PIN;
  buttons_pins[7]  = KEY_7_PIN;
  buttons_pins[8]  = KEY_8_PIN;
  buttons_pins[9]  = KEY_9_PIN;
  buttons_pins[10] = KEY_10_PIN;
  buttons_pins[11] = KEY_11_PIN;

  frequency_values[0] = 523.2;
  frequency_values[1] = 555.9;
  frequency_values[2] = 588.6;
  frequency_values[3] = 621.3;
  frequency_values[4] = 654;
  frequency_values[5] = 686.7;
  frequency_values[6] = 719.4;
  frequency_values[7] = 784.8;
  frequency_values[8] = 817.5;
  frequency_values[9] = 850.2;
  frequency_values[10] = 915.6;
  frequency_values[11] = 981; 
    
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    pinMode(buttons_pins[i], INPUT);
    
    buttons_state[i] = 0;

    osc[i].setTable(SIN2048_DATA);
    osc[i].setFreq(frequency_values[i]);
  }
}
