#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include "pinout.h"
#define CONTROL_RATE 32

#define DEBUG                   0
#define NUMBER_OF_BUTTONS       12
#define KEY_ACTIVE              LOW

byte buttons_pins[NUMBER_OF_BUTTONS];
byte buttons_state[NUMBER_OF_BUTTONS];
float state_frequencies[NUMBER_OF_BUTTONS];
float frequency_values[NUMBER_OF_BUTTONS];
Oscil<2048, AUDIO_RATE> osc[NUMBER_OF_BUTTONS];

float base_frequency = 880;
char binary[6] = {0};

void setup() {
  #if DEBUG
  Serial.begin(9600);
  #endif
  Serial.begin(9600);
  initialize();
  startMozzi(CONTROL_RATE);
  adcReconnectAllDigitalIns();
}

void updateControl() {
  updateButtons();
}

int updateAudio() {
  return  (buttons_state[0] * osc[0].next()
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
                + buttons_state[11] * osc[11].next())  >> 4;
}

void loop() {
  audioHook();
}

void updateButtons() {
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    
    if (digitalRead(buttons_pins[i]) == KEY_ACTIVE)
      buttons_state[i] = !KEY_ACTIVE;
    else
      buttons_state[i] = KEY_ACTIVE;      
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
  assign_pins();  
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
   pinMode(buttons_pins[i], INPUT);    
    buttons_state[i] = !KEY_ACTIVE;
    setFrequencies(i);
  }
}

void setFrequencies(int i) {
    frequency_values[i] = base_frequency * pow(2, i/12.0);
    osc[i].setTable(SIN2048_DATA);
    osc[i].setFreq(frequency_values[i]);
}

void assign_pins() {
  buttons_pins[0] = KEY_0_PIN;
  buttons_pins[1] = KEY_1_PIN;
  buttons_pins[2] = KEY_2_PIN;
  buttons_pins[3] = KEY_3_PIN;
  buttons_pins[4] = KEY_4_PIN;
  buttons_pins[5] = KEY_5_PIN;
  buttons_pins[6] = KEY_6_PIN;
  buttons_pins[7] = KEY_7_PIN;
  buttons_pins[8] = KEY_8_PIN;
  buttons_pins[9] = KEY_9_PIN;
  buttons_pins[10] = KEY_10_PIN;
  buttons_pins[11] = KEY_11_PIN;
}
