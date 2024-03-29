int enc_pan_state_a;
int enc_pan_state_b;
int enc_tilt_state_a;
int enc_tilt_state_b;

long x_encoder_position;
long y_encoder_position;

boolean tracking;

void init_encoders(){
  pinMode(ENC_PAN_A, INPUT);
  pinMode(ENC_PAN_B, INPUT);

  pinMode(ENC_TILT_A, INPUT);
  pinMode(ENC_TILT_B, INPUT);

  x_encoder_position = 0;
  y_encoder_position = 0;
  
  enc_pan_state_a = digitalRead(ENC_PAN_A);
  enc_pan_state_b = digitalRead(ENC_PAN_B);

  enc_tilt_state_a = digitalRead(ENC_TILT_A);
  enc_tilt_state_b = digitalRead(ENC_TILT_B);

  tracking = false;
}

void update_encoders(){
  boolean moved = false;
  
  int pan_a = digitalRead(ENC_PAN_A);
  int pan_b = digitalRead(ENC_PAN_B);

  int tilt_a = digitalRead(ENC_TILT_A);
  int tilt_b = digitalRead(ENC_TILT_B);

  //Detect edges
  if(pan_a == HIGH && enc_pan_state_a == LOW){
    if(pan_b == LOW){
      x_encoder_position++;
      moved = true;
    } else {
      x_encoder_position--;
      moved = true;
    }
  } else if(pan_a == LOW && enc_pan_state_a == HIGH){
    if(pan_b == HIGH){
      x_encoder_position++;
      moved = true;
    } else {
      x_encoder_position--;
      moved = true;
    }
  } else if(pan_b == HIGH && enc_pan_state_b == LOW){
    if(pan_a == HIGH){
      x_encoder_position++;
      moved = true;
    } else {
      x_encoder_position--;
      moved = true;
    }
  } else if(pan_b == LOW && enc_pan_state_b == HIGH){
    if(pan_a == LOW){
      x_encoder_position++;
      moved = true;
    } else {
      x_encoder_position--;
      moved = true;
    }
  }

  if(tilt_a == HIGH && enc_tilt_state_a == LOW){
    if(tilt_b == LOW){
      y_encoder_position++;
      moved = true;
    } else {
      y_encoder_position--;
      moved = true;
    }
  } else if(tilt_a == LOW && enc_tilt_state_a == HIGH){
    if(tilt_b == HIGH){
      y_encoder_position++;
      moved = true;
    } else {
      y_encoder_position--;
      moved = true;
    }
  } else if(tilt_b == HIGH && enc_tilt_state_b == LOW){
    if(tilt_a == HIGH){
      y_encoder_position++;
      moved = true;
    } else {
      y_encoder_position--;
      moved = true;
    }
  } else if(tilt_b == LOW && enc_tilt_state_b == HIGH){
    if(tilt_a == LOW){
      y_encoder_position++;
      moved = true;
    } else {
      y_encoder_position--;
      moved = true;
    }
  }

  if (tracking && moved){
    float x_deg = get_x_degrees(x_encoder_position * X_STEPS_PER_ENC_PULSE);
    float y_deg = get_y_degrees(y_encoder_position * Y_STEPS_PER_ENC_PULSE);
    Serial.print(x_deg, 8);
    Serial.print(", ");
    Serial.println(y_deg, 8);
  }
  
  enc_pan_state_a = pan_a;
  enc_pan_state_b = pan_b;

  enc_tilt_state_a = tilt_a;
  enc_tilt_state_b = tilt_b;
}

long get_x_steps(float deg) {
  deg -= X_MIN;
  float steps = deg * X_STEPS_PER_DEG;
  return round(steps);
}

long get_x_encoder_pos(){
  return x_encoder_position;
}

long get_y_encoder_pos(){
  return y_encoder_position;
}

long get_y_steps(float deg) {
  deg -= Y_MIN;
  float steps = deg * Y_STEPS_PER_DEG;
  return round(steps);
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

void tracking_on(){
  tracking = true;
}

void tracking_off(){
  tracking = false;
}

