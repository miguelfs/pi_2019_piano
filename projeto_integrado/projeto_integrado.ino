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

Oscil<2048, AUDIO_RATE> osc[NUMBER_OF_BUTTONS];

float currentSample = 0; //sample value to be outputed

float base_frequency = 880;

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

//observacao:
//oscil.next() Updates the phase according to the current frequency and
//returns the sample at the new phase position.
int updateAudio(){
   int pressedButtonsCounter = 0;
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    if (buttons_state[i] == KEY_ACTIVE) {
      currentSample += osc[i].next();
      pressedButtonsCounter++;
    }
  }

 #if DEBUG
  Serial.print("Amplitude na saida:");
  Serial.print(" ");
  Serial.print((int) (currentSample)>>2);
 #endif
  
  return (int) (currentSample)>>2;
  //dividi por 4 pq seila, talvez role saturacao e seja melhor dividir por 8
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

  pinMode(TONE_PIN, OUTPUT);
}
