#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <EventDelay.h>

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
ADSR <CONTROL_RATE,AUDIO_RATE> envelope[NUMBER_OF_BUTTONS];
EventDelay eventDelay;

int lastButtonState;

void setup() {
  #if DEBUG
  Serial.begin(115200);
  #endif
  
  initialize();
  startMozzi(CONTROL_RATE); 
}

void updateControl(){
  updateButtons();
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    envelope[i].update();
}

void loop(){
  audioHook();
}

// updateAudio:
// Returns sound played, considering polyphonic tunes.
// Since for loop screws with computational time, the best choice is to sum all the values, multiplied by their button state (1 or 0);
// Bitwise operation used for fast division in order to decrease output saturation.
int updateAudio(){
  return( (long) envelope[0].next() * osc[0].next()
                + envelope[1].next() * osc[1].next()
                + envelope[2].next() * osc[2].next() 
                + envelope[3].next() * osc[3].next() 
                + envelope[4].next() * osc[4].next() 
             // + envelope[5].next() * osc[5].next()
                ) >> 9;
}

void updateButtons() {
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    if (digitalRead(buttons_pins[i]) == KEY_ACTIVE) {
      // 1 -> Button pressed, 0 otherwise
      if (buttons_state[i] == KEY_ACTIVE) {
       envelope[i].noteOn();
      }        
    }
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
     envelope[i].setTimes(70,200,400,300);
     envelope[i].setADLevels(255,128);
     envelope[i].setSustainLevel(64);
     envelope[i].setReleaseLevel(0);
  }
  
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    pinMode(buttons_pins[i], INPUT);
    
    buttons_state[i] = KEY_ACTIVE;

    //frequency_multipliers[i] = pow(2, i/12.0);
    //frequency_values[i] = base_frequency * frequency_multipliers[i];
    
    frequency_values[0] = 440;
    frequency_values[1] = 495;
    frequency_values[2] = 550;
    frequency_values[3] = 586.67;
    frequency_values[4] = 660;
    frequency_values[5] = 733.33;
    
    osc[i].setTable(SIN2048_DATA);
    osc[i].setFreq(frequency_values[i]);
  }
}
