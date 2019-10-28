long x_step_position;
long y_step_position;
long x_steps_max;
long y_steps_max;

int x_dir;
int y_dir;
int x_dir_prev;
int y_dir_prev;
int x_state;
int y_state;
unsigned long last_check;
boolean x_motor_enabled;
boolean y_motor_enabled;

boolean x_homing;
boolean y_homing;

unsigned long x_periods_passed;
unsigned long y_periods_passed;
unsigned long x_wait_periods;
unsigned long y_wait_periods;

boolean position_control;

long x_target_pos;
long y_target_pos;
float x_target_pos_deg;
float y_target_pos_deg;

float x_vel;
float y_vel;

void init_motors() {
  x_step_position = 0;
  y_step_position = 0;

  x_steps_max = get_x_steps(X_MAX);
  y_steps_max = get_y_steps(Y_MAX);
  
  x_dir = 0;
  y_dir = 0;
  x_dir_prev = x_dir;
  y_dir_prev = y_dir;

  x_state = LOW;
  y_state = LOW;

  last_check = 0;

  x_motor_enabled = false;
  y_motor_enabled = false;

  x_homing = false;
  y_homing = false;
  
  x_periods_passed = 0;
  y_periods_passed = 0;
  x_wait_periods = 0;
  y_wait_periods = 0;

  position_control = false;

  x_target_pos = 0;
  y_target_pos = 0;
  x_target_pos_deg = X_MIN;
  y_target_pos_deg = Y_MIN;

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
  digitalWrite(X_ENABLE_PIN, DISABLE);
  digitalWrite(Y_ENABLE_PIN, DISABLE);
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

unsigned long get_x_step_pos() {
  return x_step_position;
}

unsigned long get_y_step_pos() {
  return y_step_position;
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
  if (position_control) {
    if (x_target_pos > x_step_position) {
      x_dir = 1;
    } else if (x_target_pos < x_step_position) {
      x_dir = -1;
    } else {
      x_dir = 0;
    }
    if (y_target_pos > y_step_position) {
      y_dir = 1;
    } else if (y_target_pos < y_step_position) {
      y_dir = -1;
    } else {
      y_dir = 0;
    }
    if(x_target_pos == x_step_position && x_dir_prev != 0){
      if (x_homing){
        x_homing = false;
        x_encoder_position = 0;
      }
      Serial.println("x_target_reached");
    }
    if(y_target_pos == y_step_position && y_dir_prev != 0){
      if (y_homing){
        y_homing = false;
        y_encoder_position = 0;
      }
      Serial.println("y_target_reached");
    }
  }

  if(x_dir == 1 && x_step_position >= x_steps_max){
    x_dir = 0;
  } else if(x_dir == -1 && x_step_position <= 0){
    x_dir = 0;
  }

  if(y_dir == 1 && y_step_position >= y_steps_max){
    y_dir = 0;
  } else if(y_dir == -1 && y_step_position <= 0){
    y_dir = 0;
  }

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

void home_x(){
  enable_x_motor();
  x_homing = true;
  position_control = true;
  x_encoder_position = 0;
  x_step_position = int(X_STEPS_PER_ENC_PULSE)*2;
  x_target_pos_deg = X_MIN;
  x_target_pos = get_x_steps(x_target_pos_deg);
  x_wait_periods = speed_to_periods_x(abs(MAX_SPEED * 0.5));
}

void home_y(){
  enable_y_motor();
  y_homing = true;
  position_control = true;
  y_encoder_position = 0;
  y_step_position = int(Y_STEPS_PER_ENC_PULSE)*2;
  y_target_pos_deg = Y_MIN;
  y_target_pos = get_y_steps(y_target_pos_deg);
  y_wait_periods = speed_to_periods_y(abs(MAX_SPEED * 0.5));
}

void update_x() {
  // encoder based homing
  if (x_homing && x_encoder_position < 0){
    x_encoder_position = 0;
    x_step_position = int(X_STEPS_PER_ENC_PULSE)*2;
  }
  
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
  // encoder based homing
  if (y_homing && y_encoder_position < 0){
    y_encoder_position = 0;
    y_step_position = int(Y_STEPS_PER_ENC_PULSE)*2;
  }
  
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
  enable_x_motor();
  position_control = false;
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
  enable_y_motor();
  position_control = false;
  y_wait_periods = speed_to_periods_y(abs(vel));
  if (vel > MIN_SPEED) {
    y_dir = 1;
  } else if (vel < -MIN_SPEED) {
    y_dir = -1;
  } else {
    y_dir = 0;
  }
}

void set_x_target_pos(float x_deg) {
  enable_x_motor();
  x_deg = min(x_deg, X_MAX);
  x_deg = max(x_deg, X_MIN);
  position_control = true;
  x_target_pos_deg = x_deg;
  x_target_pos = get_x_steps(x_deg);
  x_wait_periods = speed_to_periods_x(abs(MAX_SPEED));
}

void set_y_target_pos(float y_deg) {
  enable_y_motor();
  y_deg = min(y_deg, Y_MAX);
  y_deg = max(y_deg, Y_MIN);
  position_control = true;
  y_target_pos_deg = y_deg;
  y_target_pos = get_y_steps(y_deg);
  y_wait_periods = speed_to_periods_y(abs(MAX_SPEED));
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

boolean homing(){
  if (x_homing){
    return true;
  }
  if (y_homing){
    return true;
  }
  return false;
}

