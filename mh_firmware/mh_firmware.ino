#include "pins.h"
#include "configuration.h"

void setup() {
  init_motors();
  init_light();

  Serial.begin(BAUDRATE);
}

void loop() {
  // read new commands
  while(Serial.available() > 0){
    String instring = Serial.readStringUntil('\n');
    interpret_command(instring);
  }
  update_motors();
  update_light();
}

