int x_dir = 0;
int y_dir = 0;
int x_dir_prev = x_dir;
int y_dir_prev = y_dir;

int x_state = LOW;
int y_state = LOW;

unsigned long last_check = 0;

boolean motors_enabled = true;

void init_motors(){
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);

  pinMode(E0_STEP_PIN, OUTPUT);
  pinMode(E0_DIR_PIN, OUTPUT);
  pinMode(E0_ENABLE_PIN, OUTPUT);

  pinMode(E1_STEP_PIN, OUTPUT);
  pinMode(E1_DIR_PIN, OUTPUT);
  pinMode(E1_ENABLE_PIN, OUTPUT);

  digitalWrite(X_DIR_PIN, X_DIRECTION);
  digitalWrite(Y_DIR_PIN, Y_DIRECTION);
  digitalWrite(X_ENABLE_PIN, ENABLE);
  digitalWrite(Y_ENABLE_PIN, ENABLE);
}

void toggle_motors() {
  if (motors_enabled) {
    digitalWrite(X_ENABLE_PIN, DISABLE);
    digitalWrite(Y_ENABLE_PIN, DISABLE);
    motors_enabled = false;
  } else {
    digitalWrite(X_ENABLE_PIN, ENABLE);
    digitalWrite(Y_ENABLE_PIN, ENABLE);
    motors_enabled = true;
  }
}

void update_motors(){
  // switch direction
  if(x_dir_prev != x_dir){
    if(x_dir == 1){
      digitalWrite(X_DIR_PIN, X_DIRECTION);
    } else {
      digitalWrite(X_DIR_PIN, !X_DIRECTION);
    }
  }

  if(y_dir_prev != y_dir){
    if(y_dir == 1){
      digitalWrite(Y_DIR_PIN, Y_DIRECTION);
    } else {
      digitalWrite(Y_DIR_PIN, !Y_DIRECTION);
    }
  }
  
  long delta = micros() - last_check;
  if (delta > UPDATE_PERIOD) {
    if(abs(x_dir) > 0){
      digitalWrite(X_STEP_PIN, !x_state);
      x_state = !x_state;
    }

    if(abs(y_dir) > 0){
      digitalWrite(Y_STEP_PIN, !y_state);
      y_state = !y_state;
    }
    
    last_check = micros();
  }

  x_dir_prev = x_dir;
  y_dir_prev = y_dir;
}

void set_x_vel(float vel){
  if(vel > 0.1f){
    x_dir = 1;
  } else if (vel < -0.1f){
    x_dir = -1;
  } else {
    x_dir = 0;
  }
}

void set_y_vel(float vel){
  if(vel > 0.1f){
    y_dir = 1;
  } else if (vel < -0.1f){
    y_dir = -1;
  } else {
    y_dir = 0;
  }
}
