boolean interpret_command(String cmd) {
  cmd.trim();
  int spaces = count_char(cmd, ' '); //counts number of spaces in command to check if command includes parameters
  String command;
  String param;

  if (spaces == 0) {
    command = cmd;
    param = "";
  } else {
    int to_ind = cmd.indexOf(' ');
    command = cmd.substring(0, to_ind);
    param = cmd.substring(to_ind + 1);
  }

  if (command.equals("x_vel")) {
    if (string_is_float(param)) {
      set_x_vel(param.toFloat());
      return true;
    }
  } else if (command.equals("y_vel")) {
    if (string_is_float(param)) {
      set_y_vel(param.toFloat());
      return true;
    }
  } else if (command.equals("x_pos")) {
    if (string_is_float(param)) {
      set_x_target_pos(param.toFloat());
      return true;
    }
  } else if (command.equals("y_pos")) {
    if (string_is_float(param)) {
      set_y_target_pos(param.toFloat());
      return true;
    }
  } else if (command.equals("speed")) {
    if (string_is_float(param)) {
      set_movement_speed(param.toFloat());
      return true;
    }
  } else if (command.equals("home")) {
    home_x();
    home_y();
    return true; 
  } else if (command.equals("x_home")) {
    home_x();
    return true; 
  } else if (command.equals("y_home")) {
    home_y();
    return true;
  } else if (command.equals("led_on")) {
    enable_light();
    return true;
  } else if (command.equals("led_off")) {
    disable_light();
    return true;
  } else  if (command.equals("x_on")) {
    enable_x_motor();
    return true;
  } else if (command.equals("x_off")) {
    disable_x_motor();
    return true;
  }  else  if (command.equals("y_on")) {
    enable_y_motor();
    return true;
  } else if (command.equals("y_off")) {
    disable_y_motor();
    return true;
  }  else  if (command.equals("motors_on")) {
    enable_motors();
    return true;
  } else if (command.equals("motors_off")) {
    disable_motors();
    return true;
  } else if (command.equals("led_int")) {
    if (string_is_int(param)) {
      set_intensity(param.toInt());
      return true;
    }
  } else if (command.equals("get_pos")) {
    float x_deg = get_x_degrees(get_x_encoder_pos() * X_STEPS_PER_ENC_PULSE);
    float y_deg = get_y_degrees(get_y_encoder_pos() * Y_STEPS_PER_ENC_PULSE);
    Serial.print(x_deg, 8);
    Serial.print(", ");
    Serial.println(y_deg, 8);
    return true;
  } else if (command.equals("get_enc_pos")) {
    long x_enc = get_x_encoder_pos();
    long y_enc = get_y_encoder_pos();
    Serial.print(x_enc);
    Serial.print(", ");
    Serial.println(y_enc);
    return true;
  } else if (command.equals("strobe_on")) {
    enable_strobe();
    return true;
  } else if (command.equals("strobe_off")) {
    disable_strobe();
    return true;
  } else if (command.equals("tracking_on")) {
    tracking_on();
    return true;
  } else if (command.equals("tracking_off")) {
    tracking_off();
    return true;
  }

  return false;
}
