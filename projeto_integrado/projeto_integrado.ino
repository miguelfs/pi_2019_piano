#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>

#include "pinout.h"

#define CONTROL_RATE 128

#define DEBUG                   1
#define NUMBER_OF_BUTTONS       6
#define KEY_ACTIVE              LOW

int buttons_pins[NUMBER_OF_BUTTONS];
int buttons_state[NUMBER_OF_BUTTONS];
float frequency_multipliers[NUMBER_OF_BUTTONS];
float frequency_values[NUMBER_OF_BUTTONS];

float base_frequency = 880;

Oscil<2048, AUDIO_RATE> osc[NUMBER_OF_BUTTONS];

float outputSample = 0;

void setup() {
  #if DEBUG
  Serial.begin(9600);
  #endif
  Serial.begin(9600);
  initialize();
  startMozzi(CONTROL_RATE); 
}

void updateControl(){
  updateButtons();
}

void loop(){
    audioHook();
}


int updateAudio(){
  outputSample = 0;
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    if (buttons_state[i] == KEY_ACTIVE) {
      outputSample += osc[i].next();
    }
  }

 #if DEBUG
  Serial.print("Amplitude na saida:");
  Serial.print(" ");
  Serial.print((int) (outputSample)>>3);
 #endif
  
  return (int) (outputSample)>>3;
}

void updateButtons() {
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    if (digitalRead(buttons_pins[i]) == KEY_ACTIVE)
      buttons_state[i] = KEY_ACTIVE;
    else
      buttons_state[i] = !KEY_ACTIVE;      
  }

  #if DEBUG

  Serial.print("Estado dos botoes:");
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    Serial.print(" ");
    Serial.print(buttons_state[i]);
  }
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
  
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    pinMode(buttons_pins[i], INPUT);
    
    buttons_state[i] = !KEY_ACTIVE;

    frequency_multipliers[i] = pow(2, i/12.0);
    frequency_values[i] = base_frequency * frequency_multipliers[i];
    osc[i].setTable(SIN2048_DATA);
    osc[i].setFreq(frequency_values[i]); 
  }
}
