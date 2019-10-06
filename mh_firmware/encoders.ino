int enc_pan_state_a;
int enc_pan_state_b;

long x_encoder_position;
long y_encoder_position;

void init_encoders(){
  pinMode(ENC_PAN_A, INPUT);
  pinMode(ENC_PAN_B, INPUT);

  x_encoder_position = 0;
  y_encoder_position = 0;
  
  enc_pan_state_a = digitalRead(ENC_PAN_A);
  enc_pan_state_b = digitalRead(ENC_PAN_B);
}

void update_encoders(){
  int pan_a = digitalRead(ENC_PAN_A);
  int pan_b = digitalRead(ENC_PAN_B);

  //Detect edges
  if(pan_a == HIGH && enc_pan_state_a == LOW){
    if(pan_b == LOW){
      x_encoder_position++;
    } else {
      x_encoder_position--;
    }
  } else if(pan_a == LOW && enc_pan_state_a == HIGH){
    if(pan_b == HIGH){
      x_encoder_position++;
    } else {
      x_encoder_position--;
    }
  } else if(pan_b == HIGH && enc_pan_state_b == LOW){
    if(pan_a == HIGH){
      x_encoder_position++;
    } else {
      x_encoder_position--;
    }
  } else if(pan_b == LOW && enc_pan_state_b == HIGH){
    if(pan_a == LOW){
      x_encoder_position++;
    } else {
      x_encoder_position--;
    }
  }

  enc_pan_state_a = pan_a;
  enc_pan_state_b = pan_b;
}

long get_x_steps(float deg) {
  deg -= X_MIN;
  float steps = deg * X_STEPS_PER_DEG;
  return round(steps);
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
