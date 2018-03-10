#include "pins.h"
#include "configuration.h"

void setup() {
  init_motors();
  init_light();

  Serial.begin(BAUDRATE);
}

void loop() {
  // read new commands
  while (Serial.available() > 0) {
    int cmd = Serial.read();
    Serial.println(cmd);
    command(cmd);
  }
  update_motors();
  update_light();
}

void command(int cmd) {
  switch (cmd) {
    case 87:
      // forward
      set_y_vel(1.0);
      break;
    case 65:
      // left
      set_x_vel(-1.0);
      break;
    case 83:
      // back
      set_y_vel(-1.0);
      break;
    case 68:
      // right
      set_x_vel(1.0);
      break;
    case 88:
      // stop X
      set_x_vel(0.0);
      break;
    case 89:
      // stop Y
      set_y_vel(0.0);
      break;
    case 72:
      toggle_motors();
      break;
  }
}

