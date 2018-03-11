boolean interpret_command(String cmd) {
  int spaces = count_char(cmd, '_'); //counts number of spaces in command to check if command includes parameters
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
    }
    return true;
  } else if (command.equals("y_vel")) {
    if (string_is_float(param)) {
      set_y_vel(param.toFloat());
    }
    return true;
  } else if (command.equals("led_on")) {
    enable_light();
    return true;
  } else if (command.equals("led_off")) {
    disable_light();
    return true;
  } else  if (command.equals("x_on")) {
    enable_x_motor();
    return true
  } else if (command.equals("x_off")) {
    disable_x_motor();
    return true
  }  else  if (command.equals("y_on")) {
    enable_y_motor();
    return true
  } else if (command.equals("y_off")) {
    disable_y_motor();
    return true
  }  else  if (command.equals("motors_on")) {
    enable_motors();
    return true
  } else if (command.equals("motors_off")) {
    disable_motors();
    return true
  }

  return false;
}
