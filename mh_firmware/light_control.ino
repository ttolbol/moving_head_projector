unsigned long last_strobe;
unsigned long strobe_period;
unsigned long strobe_pulse_length;
int light_state;
int light_int;
boolean enable_light;
boolean enable_strobe;

void init_light() {
  last_strobe = 0;
  strobe_period = DEFAULT_STROBE_PERIOD;
  strobe_pulse_length = DEFAULT_STROBE_LENGTH;
  light_int = DEFAULT_LIGHT_INT;

  enable_light = true;
  enable_strobe = false;
  light_state = LIGHT_ON;
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 255-light_int);
}

void update_light() {
  if (enable_light) {
    if (enable_strobe) {
      long delta = micros() - last_strobe;
      if (delta > strobe_period) {
        light_state = LIGHT_ON;
        analogWrite(LED_PIN, 255-light_int);
        last_strobe = micros();
      } else if (delta > strobe_pulse_length) {
        light_state = LIGHT_OFF;
        digitalWrite(LED_PIN, light_state);
      }
    } else {
      light_state = LIGHT_ON;
      analogWrite(LED_PIN, 255-light_int);
    }
  } else {
    if (light_state == LIGHT_OFF) {
      return;
    } else {
      light_state = LIGHT_OFF;
      digitalWrite(LED_PIN, light_state);
      return;
    }
  }
}
