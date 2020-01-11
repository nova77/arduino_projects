#include <Arduino.h>
#include "rgb_led.h"

// The rgb_led.
static RgbLed rgb_led(RgbLed::LT_ANODE,  //  type of LED
                      /*red_pin=*/3,
                      /*green_pin=*/5,
                      /*blue_pin=*/6);

void setup() {
  rgb_led.setup();
}

void loop() {
  rgb_led.set(/*red=*/255, /*green=*/0, /*blue=*/0);
  delay(1000);
  rgb_led.set(/*red=*/0, /*green=*/255, /*blue=*/0);
  delay(1000);
  rgb_led.set(/*red=*/0, /*green=*/0, /*blue=*/255);
  delay(1000);

  rgb_led.clear();
  delay(3000);
}
