#include "pins.h"
#include "configuration.h"

String instring;

void setup() {
  init_all();
}

void init_all(){
  instring = "";
  init_motors();
  init_light();
  init_encoders();
  Serial.begin(BAUDRATE);
}

void loop() {
  // read new commands
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == ','){
      if (interpret_command(instring)) {
        Serial.println("ok");
      }
      instring = "";
    } else {
      instring = instring + c;
    }
  }
  update_motors();
  update_light();
  update_encoders();
}
