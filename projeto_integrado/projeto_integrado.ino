#include "pinout.h"

#define DEBUG                   1
#define NUMBER_OF_BUTTONS       6
#define KEY_ACTIVE              LOW

// Set this to be the pin that your buzzer resides in. (Note that you can only have one buzzer actively using the PWM signal at a time).
int tonePin = 9;
int val = 0;   //variavel para ler status do pino

int buttons_pins[NUMBER_OF_BUTTONS];
int buttons_state[NUMBER_OF_BUTTONS];
float state_frequencies[NUMBER_OF_BUTTONS];
float frequency_multipliers[NUMBER_OF_BUTTONS];

float base_frequency = 880;

void setup() {
  #if DEBUG
  Serial.begin(9600);
  #endif
  Serial.begin(9600);
  initialize();
}


void loop()
{
  updateButtons();

  int zeroButtonsCounter = 0;
//
//  if (buttons_state[0] == KEY_ACTIVE)
//    tone(tonePin, base_frequency * frequency_multipliers[0], 1000);
//  else
//    noTone(tonePin);

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {

    if (buttons_state[i] == KEY_ACTIVE) {
      tone(tonePin, base_frequency * frequency_multipliers[i]);
      zeroButtonsCounter++;
    }
  }

  if (zeroButtonsCounter == 0)
    noTone(tonePin);
  
}
//  
//  val = digitalRead(keyE);  // read input value
//
//  if (val == 0) {         //BOTAO APERTADO
//    #if DEBUG
//    Serial.println("KeyE pressed");
//    #endif
//    
//    tone(tonePin, 329, 225.0);
//    
//     } else { // BOTAO SOLTO
//      
//        #if DEBUG
//        Serial.println("KeyE not pressed");
//        #endif
//        
//        noTone(tonePin);
//     }
//     delay(200);

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
  }

  pinMode(TONE_PIN, OUTPUT);
}
