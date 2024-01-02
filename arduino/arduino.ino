#include <ezButton.h>

ezButton button_trigger(10);  // create ezButton object that attach to pin 10;
ezButton button_delayed( 9);  // create ezButton object that attach to pin 10;

#define ENERGY_WASTE 12

#define CAM_1_FOCUS 2
#define CAM_1_TRIGGER 3

#define CAM_2_FOCUS 4
#define CAM_2_TRIGGER 5

uint32_t energy_waste_time;

void setup() {
  Serial.begin(9600);
  button_trigger.setDebounceTime(50); // set debounce time to 50 milliseconds
  button_delayed.setDebounceTime(50); // set debounce time to 50 milliseconds

  // Initialize focus and trigger lines
  for (int ii = 2; ii <= 5; ++ii) {
    pinMode(ii, OUTPUT);
    digitalWrite(ii, LOW);
  }
  // Initialize buttons
  for (int ii = 6; ii <= 10; ++ii) {
    pinMode(ii, INPUT_PULLUP);
  }

  pinMode(ENERGY_WASTE, OUTPUT);
  digitalWrite(ENERGY_WASTE, LOW);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  energy_waste_time = millis() + 5000;
}

bool button_released = true;

void trigger_high(bool cam1, bool cam2) {
  if (cam1) {
    digitalWrite(CAM_1_FOCUS, HIGH);
    digitalWrite(CAM_1_TRIGGER, HIGH);
  }
  if (cam2) {
    digitalWrite(CAM_2_FOCUS, HIGH);
    digitalWrite(CAM_2_TRIGGER, HIGH);
  }
}

void trigger_low() {
  digitalWrite(CAM_1_FOCUS, LOW);
  digitalWrite(CAM_1_TRIGGER, LOW);
  digitalWrite(CAM_2_FOCUS, LOW);
  digitalWrite(CAM_2_TRIGGER, LOW);
}

uint32_t bracket_time;
int bracket_count = 0;


void trigger() {
    trigger_high(true, true);
    _delay_ms(200);
    trigger_low();
}

void loop() {
  uint32_t const t = millis();
  button_trigger.loop(); // MUST call the loop() function first
  button_delayed.loop(); // MUST call the loop() function first

  if(button_trigger.isPressed()) {
    Serial.println("The button for trigger is pressed");
    digitalWrite(LED_BUILTIN, HIGH);
    trigger();
    digitalWrite(LED_BUILTIN, LOW);
  }

  if(button_trigger.isReleased()) {
    Serial.println("The button for trigger is released");
    digitalWrite(LED_BUILTIN, LOW);
  }

  if(button_delayed.isPressed()) {
    Serial.println("The button for delayed trigger is pressed");
    bracket_time = millis() + 5000;
    bracket_count = 5;
    digitalWrite(LED_BUILTIN, HIGH);
    _delay_ms(700);
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (bracket_count > 0) {
    if (t > bracket_time) {
      bracket_count--;
      bracket_time = t + 1000;
      digitalWrite(LED_BUILTIN, HIGH);
      trigger();
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  if (t > energy_waste_time) {
    if (LOW == digitalRead(ENERGY_WASTE)) {
      digitalWrite(ENERGY_WASTE, HIGH);
      energy_waste_time = t + 500;
    }
    else {
      digitalWrite(ENERGY_WASTE, LOW);
      energy_waste_time = t + 5000;
    }
  }

}
