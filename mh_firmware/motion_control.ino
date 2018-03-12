int x_dir;
int y_dir;
int x_dir_prev;
int y_dir_prev;
int x_state;
int y_state;
unsigned long last_check;
boolean x_motor_enabled;
boolean y_motor_enabled;

unsigned long x_step_position;
unsigned long y_step_position;
unsigned long x_periods_passed;
unsigned long y_periods_passed;
unsigned long x_wait_periods;
unsigned long y_wait_periods;

float x_vel;
float y_vel;

void init_motors() {
  x_dir = 0;
  y_dir = 0;
  x_dir_prev = x_dir;
  y_dir_prev = y_dir;

  x_state = LOW;
  y_state = LOW;

  last_check = 0;

  x_motor_enabled = true;
  y_motor_enabled = true;

  x_step_position = 0;
  y_step_position = 0;
  x_periods_passed = 0;
  y_periods_passed = 0;
  x_wait_periods = 0;
  y_wait_periods = 0;

  x_vel = 0.0f;
  y_vel = 0.0f;

  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  digitalWrite(X_DIR_PIN, X_DIRECTION);
  digitalWrite(Y_DIR_PIN, Y_DIRECTION);
  digitalWrite(X_ENABLE_PIN, ENABLE);
  digitalWrite(Y_ENABLE_PIN, ENABLE);
}

void update_motors() {
  long delta = micros() - last_check;
  if (delta > UPDATE_PERIOD) {
    update_direction();
    update_x();
    update_y();
    last_check = micros();
  }
}

unsigned long speed_to_periods_x(float s) {
  //converts deg/s to number of periods in between pulses
  //one period is equal to UPDATE_PERIOD
  if (s > MIN_SPEED) {
    float steps_per_sec = s * X_STEPS_PER_DEG;
    float period = 1000000.f / steps_per_sec;
    float n = period / UPDATE_PERIOD;
    return round(n);
  } else {
    return 0;
  }
}

unsigned long speed_to_periods_y(float s) {
  //converts deg/s to number of periods in between pulses
  //one period is equal to UPDATE_PERIOD
  if (s > MIN_SPEED) {
    float steps_per_sec = s * Y_STEPS_PER_DEG;
    float period = 1000000.f / steps_per_sec;
    float n = period / UPDATE_PERIOD;
    return round(n);
  } else {
    return 0;
  }
}

void update_direction() {
  // switch direction
  if (x_dir_prev != x_dir) {
    if (x_dir == 1) {
      digitalWrite(X_DIR_PIN, X_DIRECTION);
    } else {
      digitalWrite(X_DIR_PIN, !X_DIRECTION);
    }
  }

  if (y_dir_prev != y_dir) {
    if (y_dir == 1) {
      digitalWrite(Y_DIR_PIN, Y_DIRECTION);
    } else {
      digitalWrite(Y_DIR_PIN, !Y_DIRECTION);
    }
  }
  x_dir_prev = x_dir;
  y_dir_prev = y_dir;
}

void update_x() {
  if (x_dir != 0) {
    if (x_state == HIGH) {
      x_state = LOW;
      digitalWrite(X_STEP_PIN, x_state);
    } else {
      x_periods_passed++;
      if (x_periods_passed > x_wait_periods) {
        x_periods_passed = 0;
        x_state = HIGH;
        digitalWrite(X_STEP_PIN, x_state);
        if (x_dir == 1) {
          x_step_position++;
        } else {
          x_step_position--;
        }
      }
    }
  }
}

void update_y() {
  if (y_dir != 0) {
    if (y_state == HIGH) {
      y_state = LOW;
      digitalWrite(Y_STEP_PIN, y_state);
    } else {
      y_periods_passed++;
      if (y_periods_passed > y_wait_periods) {
        y_periods_passed = 0;
        y_state = HIGH;
        digitalWrite(Y_STEP_PIN, y_state);
        if (y_dir == 1) {
          y_step_position++;
        } else {
          y_step_position--;
        }
      }
    }
  }
}

void set_x_vel(float vel) {
  x_wait_periods = speed_to_periods_x(abs(vel));
  if (vel > MIN_SPEED) {
    x_dir = 1;
  } else if (vel < -MIN_SPEED) {
    x_dir = -1;
  } else {
    x_dir = 0;
  }
}

void set_y_vel(float vel) {
  y_wait_periods = speed_to_periods_y(abs(vel));
  if (vel > MIN_SPEED) {
    y_dir = 1;
  } else if (vel < -MIN_SPEED) {
    y_dir = -1;
  } else {
    y_dir = 0;
  }
}

void enable_x_motor() {
  x_motor_enabled = true;
  digitalWrite(X_ENABLE_PIN, ENABLE);
}

void enable_y_motor() {
  y_motor_enabled = true;
  digitalWrite(Y_ENABLE_PIN, ENABLE);
}

void disable_x_motor() {
  x_motor_enabled = false;
  digitalWrite(X_ENABLE_PIN, DISABLE);
}

void disable_y_motor() {
  y_motor_enabled = false;
  digitalWrite(Y_ENABLE_PIN, DISABLE);
}

void enable_motors() {
  enable_x_motor();
  enable_y_motor();
}

void disable_motors() {
  disable_x_motor();
  disable_y_motor();
}

unsigned long get_x_steps() {
  return x_step_position;
}

unsigned long get_y_steps() {
  return y_step_position;
}

float get_x_degrees(unsigned long steps) {
  float pos = X_MIN;
  pos += steps / X_STEPS_PER_DEG;
  return pos;
}

float get_y_degrees(unsigned long steps) {
  float pos = Y_MIN;
  pos += steps / Y_STEPS_PER_DEG;
  return pos;
}
