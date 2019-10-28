unsigned long last_strobe;
unsigned long strobe_period;
unsigned long strobe_pulse_length;
int light_state;
int light_int;
boolean light_enabled;
boolean strobe_enabled;

void init_light() {
  last_strobe = 0;
  strobe_period = DEFAULT_STROBE_PERIOD;
  strobe_pulse_length = DEFAULT_STROBE_LENGTH;
  light_int = DEFAULT_LIGHT_INT;

  light_enabled = true;
  strobe_enabled = false;
  light_state = LIGHT_ON;
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 255 - light_int);
}

void update_light() {
  if (light_enabled) {
    if (strobe_enabled) {
      long delta = micros() - last_strobe;
      if (delta > strobe_period) {
        light_state = LIGHT_ON;
        analogWrite(LED_PIN, 255 - light_int);
        last_strobe = micros();
      } else if (delta > strobe_pulse_length) {
        light_state = LIGHT_OFF;
        digitalWrite(LED_PIN, light_state);
      }
    } else {
      light_state = LIGHT_ON;
      analogWrite(LED_PIN, 255 - light_int);
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

void enable_light() {
  light_enabled = true;
}

void disable_light() {
  light_enabled = false;
}

void toggle_light() {
  light_enabled = !light_enabled;
}

void enable_strobe() {
  strobe_enabled = true;
}

void disable_strobe() {
  strobe_enabled = false;
}

void toggle_strobe() {
  strobe_enabled = !strobe_enabled;
}

void set_intensity(int intensity) {
  if(intensity > 255){
    intensity = 255;
  } else if(intensity < 0){
    intensity = 0;
  }
  light_int = intensity;
}

int get_intensity() {
  return light_int;
}

