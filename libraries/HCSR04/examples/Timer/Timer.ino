#include "hcsr04.h"
#include <TimerOne.h>

HCSR02 ultrasonic(3,2);

void serial () {
  Serial.println("got it");
}

void setup() {
  Serial.begin(115200);
  ultrasonic.setup();

  Timer1.initialize(1);
  Timer1.attachInterrupt(checkDistance);
}

void loop() {
  Serial.println(ultrasonic.getDistance());
  
  delay(1);
}

void checkDistance () {
  if (inRange()) {
    serial();
  }
}

bool inRange () {
  return 0 < ultrasonic.getDistance() && ultrasonic.getDistance() <= 10;
}
