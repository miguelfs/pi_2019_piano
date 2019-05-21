#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <EventDelay.h>

#include <tables/sin2048_int8.h>
#include "pinout.h"

#define CONTROL_RATE            128
#define DEBUG                   0
#define NUMBER_OF_BUTTONS       12
#define KEY_ACTIVE              LOW

int buttons_pins[NUMBER_OF_BUTTONS];
//int buttons_state[NUMBER_OF_BUTTONS];
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
                + envelope[5].next() * osc[5].next()
                + envelope[6].next() * osc[6].next()
                + envelope[7].next() * osc[7].next() 
                + envelope[8].next() * osc[8].next() 
                + envelope[9].next() * osc[9].next()
                + envelope[10].next() * osc[10].next() 
                + envelope[11].next() * osc[11].next() 
                ) >> 10;
}

void updateButtons() {

  #if DEBUG
  Serial.print("Estado dos botoes: ");
  #endif
  
  int howManyButtonsArePressed = 0;
  
  for (int i = 0; i < NUMBER_OF_BUTTONS-1; i++) {
    
    if (digitalRead(buttons_pins[i]) == KEY_ACTIVE) {
      
      if (howManyButtonsArePressed > 2) {
        envelope[i].noteOff();
        howManyButtonsArePressed--;
      }
      else {
        envelope[i].noteOn();
        howManyButtonsArePressed++;
      }
    }

    
    
    #if DEBUG
    Serial.print(" ");
    Serial.print(buttons_state[i]);
    #endif
    
  }
  
  if (analogRead(buttons_pins[11]) < 10) {
      envelope[11].noteOn();
    }

  #if DEBUG
  Serial.println();
  #endif
}

void initialize() {
  buttons_pins[0]  = KEY_0_PIN;
  buttons_pins[1]  = KEY_1_PIN;
  buttons_pins[2]  = KEY_2_PIN;
  buttons_pins[3]  = KEY_3_PIN;
  buttons_pins[4]  = KEY_4_PIN;
  buttons_pins[5]  = KEY_5_PIN;
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
     envelope[i].setTimes(70,200,400,300);
     envelope[i].setADLevels(255,128);
     envelope[i].setSustainLevel(64);
     envelope[i].setReleaseLevel(0);
  }
  
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    pinMode(buttons_pins[i], INPUT);

    //frequency_multipliers[i] = pow(2, i/12.0);
    //frequency_values[i] = base_frequency * frequency_multipliers[i];
    
    osc[i].setTable(SIN2048_DATA);
    osc[i].setFreq(frequency_values[i]);
  }
}
