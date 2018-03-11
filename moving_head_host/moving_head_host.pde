import processing.serial.*;

int BAUD_RATE = 115200;
int COM_PORT = 0;

Serial port;

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

int nl = 10; //new line is 10 in ascii

void setup() {
  size(600, 600);

  printArray(Serial.list());
  port = new Serial(this, Serial.list()[COM_PORT], BAUD_RATE);
  port.clear();

  rectMode(CENTER);
  background(0);
  noStroke();
}

void draw() {
  String in_string = null;
  while (port.available() > 0) {
    in_string = port.readStringUntil(nl);
    if (in_string != null) {
      println(in_string);
    }
  }
  
  //update();
  render();
}

void update() {
  if (light_on != light_on_prev) {
    if (light_on) {
      port.write(32);
    } else {
      port.write(33);
    }
  }

  x_dir = right-left;
  y_dir = up-down;

  if (x_dir != x_dir_prev) {
    switch(x_dir) {
    case 1:
      port.write(68);
      break;
    case -1:
      port.write(65);
      break;
    default:
      port.write(88);
      break;
    }
  }

  if (y_dir != y_dir_prev) {
    switch(y_dir) {
    case 1:
      port.write(87);
      break;
    case -1:
      port.write(83);
      break;
    default:
      port.write(89);
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
    println("coded");
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
  
  update();
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
  }  else if (key == ' ') {
    light_on = false;
  }
  
  update();
}