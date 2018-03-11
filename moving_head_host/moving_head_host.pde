import net.java.games.input.*;
import org.gamecontrolplus.*;
import org.gamecontrolplus.gui.*;

import processing.serial.*;

boolean connected = false;

int BAUD_RATE = 115200;
int COM_PORT = 0;

Serial port;

boolean enable_joystick = false;
ControlIO control;
ControlDevice device;
ControlSlider x_axis;
ControlSlider x_axis_alt;
ControlSlider y_axis;
ControlSlider throttle;
ControlButton trigger;

int x_dir = 0;
int y_dir = 0;

int x_dir_prev = x_dir;
int y_dir_prev = y_dir;

boolean light_on = false;
boolean light_on_prev = false;

int up = 0;
int down = 0;
int left = 0;
int right = 0;

float x_val = 0.0f;
float y_val = 0.0f;

int nl = 10; //new line is 10 in ascii

void setup() {
  size(600, 600);

  control = ControlIO.getInstance(this);
  if (control.getNumberOfDevices() > 0) {
    enable_joystick = true;
    device = control.getDevice(0);
    x_axis = device.getSlider(0);
    x_axis_alt = device.getSlider(5);
    y_axis = device.getSlider(1);
    throttle = device.getSlider(2);
    trigger = device.getButton(0);
  }

  printArray(Serial.list());
  if (Serial.list()[COM_PORT].equals("/dev/ttyACM0")) {
    port = new Serial(this, Serial.list()[COM_PORT], BAUD_RATE);
    port.clear();
    connected = true;
    println("CONNECTED!");
  }

  rectMode(CENTER);
  background(0);
  noStroke();
}

void draw() {
  if (connected) {
    read_serial();
  }

  if (enable_joystick) {
    update_joystick();
  }
  render();
}

void read_serial() {
  String in_string = null;
  while (port.available() > 0) {
    in_string = port.readStringUntil(nl);
    if (in_string != null) {
      println(in_string);
    }
  }
}

void update_joystick() {
  x_val = x_axis_alt.getValue();
  y_val = y_axis.getValue();

  x_dir = 0;
  y_dir = 0;
  light_on = trigger.pressed();

  if (x_val > 0.1f) {
    x_dir = 1;
  } else if (x_val < -0.1f) {
    x_dir = -1;
  }

  if (y_val > 0.1f) {
    y_dir = 1;
  } else if (y_val < -0.1f) {
    y_dir = -1;
  }
  
  if (light_on != light_on_prev) {
    if (light_on) {
      serial_send(32);
    } else {
      serial_send(33);
    }
  }
  if (x_dir != x_dir_prev) {
    switch(x_dir) {
    case 1:
      serial_send(68);
      right = 1;
      left = 0;
      break;
    case -1:
      serial_send(65);
      right = 0;
      left = 1;
      break;
    default:
      serial_send(88);
      right = 0;
      left = 0;
      break;
    }
  }
  if (y_dir != y_dir_prev) {
    switch(y_dir) {
    case 1:
      serial_send(87);
      up = 1;
      down = 0;
      break;
    case -1:
      serial_send(83);
      up = 0;
      down = 1;
      break;
    default:
      serial_send(89);
      up = 0;
      down = 0;
      break;
    }
  }


  x_dir_prev = x_dir;
  y_dir_prev = y_dir;
  light_on_prev = light_on;
}

void serial_send(int b) {
  if (connected) {
    port.write(b);
  }
}

void update_keyboard() {
  x_dir = right-left;
  y_dir = up-down;

  if (light_on != light_on_prev) {
    if (light_on) {
      serial_send(32);
    } else {
      serial_send(33);
    }
  }
  if (x_dir != x_dir_prev) {
    switch(x_dir) {
    case 1:
      serial_send(68);
      break;
    case -1:
      serial_send(65);
      break;
    default:
      serial_send(88);
      break;
    }
  }
  if (y_dir != y_dir_prev) {
    switch(y_dir) {
    case 1:
      serial_send(87);
      break;
    case -1:
      serial_send(83);
      break;
    default:
      serial_send(89);
      break;
    }
  }

  x_dir_prev = x_dir;
  y_dir_prev = y_dir;
  light_on_prev = light_on;
}

void render() {
  background(0);
  if (up == 1) {
    fill(255);
    rect(300, 204, 64, 64);
  } else {
    fill(32);
    rect(300, 204, 64, 64);
  }
  if (down == 1) {
    fill(255);
    rect(300, 396, 64, 64);
  } else {
    fill(32);
    rect(300, 396, 64, 64);
  }
  if (left == 1) {
    fill(255);
    rect(204, 300, 64, 64);
  } else {
    fill(32);
    rect(204, 300, 64, 64);
  }
  if (right == 1) {
    fill(255);
    rect(396, 300, 64, 64);
  } else {
    fill(32);
    rect(396, 300, 64, 64);
  }
  if (light_on) {
    fill(255);
    ellipse(300, 300, 100, 100);
  } else {
    fill(32);
    ellipse(300, 300, 100, 100);
  }
}

void keyPressed() {
  if (key == CODED) {
    switch (keyCode) {
    case UP:
      up = 1;
      break;
    case DOWN:
      down = 1;
      break;
    case LEFT:
      left = 1;
      break;
    case RIGHT:
      right = 1;
      break;
    }
  } else if (key == ' ') {
    light_on = true;
  }

  update_keyboard();
}

void keyReleased() {
  if (key == CODED) {
    switch (keyCode) {
    case UP:
      up = 0;
      break;
    case DOWN:
      down = 0;
      break;
    case LEFT:
      left = 0;
      break;
    case RIGHT:
      right = 0;
      break;
    }
  } else if (key == ' ') {
    light_on = false;
  }

  update_keyboard();
}